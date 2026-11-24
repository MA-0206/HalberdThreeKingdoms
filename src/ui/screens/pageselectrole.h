#ifndef PAGESELECTROLE_H
#define PAGESELECTROLE_H

#include <QWidget>
#include "roleselectwidget.h"

namespace Ui {
class PageSelectRole;
}

class PageSelectRole : public QWidget
{
    Q_OBJECT

public:
    explicit PageSelectRole(QWidget *parent = nullptr);
    ~PageSelectRole();

signals:
    void roleConfirmed(const QString& roleName);

private:
    Ui::PageSelectRole *ui;

private:
    RoleSelectWidget* liubeiWidget;
    RoleSelectWidget* guanyuWidget;

    QString m_selectedRole = "";
    void initSelectWidget();
    void selectRole();
};

#endif // PAGESELECTROLE_H
