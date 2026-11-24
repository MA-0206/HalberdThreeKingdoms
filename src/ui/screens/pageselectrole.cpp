#include "pageselectrole.h"
#include "ui_pageselectrole.h"
#include <QMessageBox>

PageSelectRole::PageSelectRole(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageSelectRole)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    initSelectWidget();
    selectRole();
}

void PageSelectRole::initSelectWidget()
{
    liubeiWidget = new RoleSelectWidget("liubei", this);
    guanyuWidget = new RoleSelectWidget("guanyu", this);

    liubeiWidget->setFixedSize(192,288);
    guanyuWidget->setFixedSize(192,288);

    liubeiWidget->move(144,213);
    guanyuWidget->move(407,213);
}

void PageSelectRole::selectRole()
{
    connect(liubeiWidget, &RoleSelectWidget::clicked, this, [=](){
        m_selectedRole = "liubei";
        guanyuWidget->setUnSelected();
    });

    connect(guanyuWidget, &RoleSelectWidget::clicked, this, [=](){
        m_selectedRole = "guanyu";
        liubeiWidget->setUnSelected();
    });

    connect(ui->btn_confirmRole, &QPushButton::clicked, this, [=](){
        if(m_selectedRole.isEmpty()){
            QMessageBox::warning(this, "提示", "请选择角色！" );
            return;
        }
        emit roleConfirmed(m_selectedRole);
    });
}

PageSelectRole::~PageSelectRole()
{
    delete ui;
}
