#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qDebug>
#include <QDir>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap mubg(":/mubg");
    if(mubg.isNull()){
        qDebug()<<"mubg upload failed!";
        painter.fillRect(0,0,this->width(),this->height(),Qt::red);
        return;
    }
    // painter.drawPixmap(0,0,this->width(),this->height(),mubg);

    // Image Anti-Aliasing
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPixmap suitbg = mubg.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    int x =(this->width() - suitbg.width())/2;
    int y =(this->height() - suitbg.height())/2;
    painter.drawPixmap(x,y,suitbg);
}
void MainWindow::on_pushButtonStartGame_clicked()
{
    this->hide();
    if(!m_gameview)
    {
        m_gameview = std::make_unique<GameView>();
        m_gameview->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    }
    m_gameview->show();
    m_gameview->setFocus();
}

