#ifndef PAGESELECTROLE_H
#define PAGESELECTROLE_H

#include <QWidget>

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
    QString m_selectedRole = "liubei";
    void selectRole();
};

#endif // PAGESELECTROLE_H
