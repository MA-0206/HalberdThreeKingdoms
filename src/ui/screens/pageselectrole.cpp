#include "pageselectrole.h"
#include "ui_pageselectrole.h"

PageSelectRole::PageSelectRole(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageSelectRole)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    selectRole();
}

void PageSelectRole::selectRole()
{
    connect(ui->btn_liubei, &QPushButton::clicked, this, [=](){
        m_selectedRole = "liubei";
        ui->btn_liubei->setStyleSheet("border: 3px solid yellow;");
        ui->btn_guanyu->setStyleSheet("border: none;");
    });

    connect(ui->btn_guanyu, &QPushButton::clicked, this, [=](){
        m_selectedRole = "guanyu";
        ui->btn_guanyu->setStyleSheet("border: 3px solid yellow;");
        ui->btn_liubei->setStyleSheet("border: none");
    });

    connect(ui->btn_confirmRole, &QPushButton::clicked, this, [=](){
        emit roleConfirmed(m_selectedRole);
    });
}

PageSelectRole::~PageSelectRole()
{
    delete ui;
}
