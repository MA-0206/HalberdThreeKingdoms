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

    this->setWindowTitle("执戈三国");
    m_pageManager = new PageManager(ui->stackedWidget,this);
    // switchPage();
    // selectRole();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// void MainWindow::switchPage()
// {
//     connect(ui->btn_startGame, &QPushButton::clicked, this, [=](){
//         ui->stackedWidget->setCurrentWidget(ui->page_selectRole);
//     });

//     connect(ui->btn_cofirmRole, &QPushButton::clicked, this, [=](){
//         ui->stackedWidget->setCurrentWidget(ui->page_worldMap);
//     });

//     connect(ui->btn_backToMain, &QPushButton::clicked, this, [=]{
//         ui->stackedWidget->setCurrentWidget(ui->page_mainMenu);
//     });
//     selectLevel();
// }

// void MainWindow::selectRole()
// {
//     connect(ui->btn_liubei, &QPushButton::clicked, this, [=](){
//         m_selectedCharacter = "liubei";
//         ui->btn_liubei->setStyleSheet("border: 3px solid yellow;");
//         ui->btn_guanyu->setStyleSheet("border: none;");
//     });

//     connect(ui->btn_guanyu, &QPushButton::clicked, this, [=](){
//         m_selectedCharacter = "guanyu";
//         ui->btn_guanyu->setStyleSheet("border: 3px solid yellow;");
//         ui->btn_liubei->setStyleSheet("border: none");
//     });
// }

// void MainWindow::selectLevel()
// {
//     connect(ui->btn_level1, &QPushButton::clicked, this, [=](){
//         GameView* game = new GameView(m_selectedCharacter, "level_1");
//         ui->stackedWidget->addWidget(game);
//         ui->stackedWidget->setCurrentWidget(game);
//     });
// }


