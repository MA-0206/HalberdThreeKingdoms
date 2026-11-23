#ifndef PAGEMAINMENU_H
#define PAGEMAINMENU_H

#include <QWidget>

namespace Ui {
class PageMainMenu;
}

class PageMainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PageMainMenu(QWidget *parent = nullptr);
    ~PageMainMenu();

private:
    Ui::PageMainMenu *ui;

private:
    void startGame();

signals:
    void requestStartGame();
};

#endif // PAGEMAINMENU_H
