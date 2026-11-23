#include "pagemainmenu.h"
#include "ui_pagemainmenu.h"

PageMainMenu::PageMainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageMainMenu)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    startGame();
}

void PageMainMenu::startGame()
{
    connect(ui->btn_startGame, &QPushButton::clicked, this, [=](){
        emit requestStartGame();
    });
}

PageMainMenu::~PageMainMenu()
{
    delete ui;
}
