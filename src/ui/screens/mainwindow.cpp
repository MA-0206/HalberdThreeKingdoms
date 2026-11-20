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

    switchPage();
    selectRole();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::paintEvent(QPaintEvent* event)
// {
//     Q_UNUSED(event);
//     QPainter painter(this);
//     QPixmap mubg(":/assets/images/ui/loading/start_background");
//     if(mubg.isNull()){
//         qDebug()<<"mubg upload failed!";
//         painter.fillRect(0,0,this->width(),this->height(),Qt::red);
//         return;
//     }

//     // Image Anti-Aliasing
//     painter.setRenderHint(QPainter::SmoothPixmapTransform);

//     QPixmap suitbg = mubg.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
//     int x =(this->width() - suitbg.width())/2;
//     int y =(this->height() - suitbg.height())/2;
//     painter.drawPixmap(x,y,suitbg);
// }

void MainWindow::switchPage()
{
    connect(ui->btn_startGame, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_selectRole);
    });

    connect(ui->btn_cofirmRole, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_worldMap);
    });

    connect(ui->btn_backToMain, &QPushButton::clicked, this, [=]{
        ui->stackedWidget->setCurrentWidget(ui->page_mainMenu);
    });
    selectLevel();
}

void MainWindow::selectRole()
{
    connect(ui->btn_liubei, &QPushButton::clicked, this, [=](){
        m_selectedCharacter = "liubei";
        ui->btn_liubei->setStyleSheet("border: 3px solid yellow;");
        ui->btn_guanyu->setStyleSheet("norder: none;");
    });

    connect(ui->btn_guanyu, &QPushButton::clicked, this, [=](){
        m_selectedCharacter = "guanyu";
        ui->btn_guanyu->setStyleSheet("border: 3px solid yellow;");
        ui->btn_liubei->setStyleSheet("border: none");
    });
}

void MainWindow::selectLevel()
{
    connect(ui->btn_level1, &QPushButton::clicked, this, [=](){
        GameView* game = new GameView(m_selectedCharacter, "level_1");
        ui->stackedWidget->addWidget(game);
        ui->stackedWidget->setCurrentWidget(game);
    });
}

// void MainWindow::on_pushButtonStartGame_clicked()
// {
//     this->hide();
//     if(!m_gameview)
//     {
//         m_gameview = std::make_unique<GameView>();
//         m_gameview->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
//     }
//     m_gameview->show();
//     m_gameview->setFocus();
// }

