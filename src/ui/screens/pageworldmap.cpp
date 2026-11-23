#include "pageworldmap.h"
#include "ui_pageworldmap.h"

PageWorldMap::PageWorldMap(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageWorldMap)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    selectLevel();
    returnMainMenu();
}

void PageWorldMap::selectLevel()
{
    connect(ui->btn_level1, &QPushButton::clicked, this, [=](){
        emit requestEnterLevel(m_currentRole,"level_1");
    });
}

void PageWorldMap::returnMainMenu()
{
    connect(ui->btn_backToMain, &QPushButton::clicked, this, [=](){
        emit requestReturnToMenu();
    });
}

PageWorldMap::~PageWorldMap()
{
    delete ui;
}
