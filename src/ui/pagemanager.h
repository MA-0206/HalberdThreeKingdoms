#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include <QStackedWidget>
#include "pagemainmenu.h"
#include "pageselectrole.h"
#include "pageworldmap.h"
#include "gameview.h"
#include "loadingwidget.h"

class PageManager : public QObject
{
    Q_OBJECT
public:
    explicit PageManager(QStackedWidget* stackWidget, QObject *parent = nullptr);
    ~PageManager();

private:
    LoadingWidget* m_loadWidget;
    QWidget* m_targetPage;

    QStackedWidget* m_stack;
    PageMainMenu* m_mainMenu;
    PageSelectRole* m_selectRole;
    PageWorldMap* m_worldMap;

    GameView* m_currentGameView = nullptr;

    QString m_currentRole;

    void initPages();
    void switchPages();
    void startBattle(const QString& levelId);
    void switchPageWithLoading(QWidget* targetPage);

signals:
};

#endif // PAGEMANAGER_H
