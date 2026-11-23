#include "pagemanager.h"

PageManager::PageManager(QStackedWidget* stackWidget, QObject *parent)
    : QObject{parent},m_stack(stackWidget)
{
    initPages();
    switchPages();
}

PageManager::~PageManager()
{

}

void PageManager::initPages()
{
    m_mainMenu = new PageMainMenu(m_stack);
    m_selectRole = new PageSelectRole(m_stack);
    m_worldMap = new PageWorldMap(m_stack);

    m_stack->addWidget(m_mainMenu);
    m_stack->addWidget(m_selectRole);
    m_stack->addWidget(m_worldMap);

    m_stack->setCurrentWidget(m_mainMenu);
}

void PageManager::switchPages()
{
    connect(m_mainMenu, &PageMainMenu::requestStartGame, this, [=](){
        m_stack->setCurrentWidget(m_selectRole);
    });

    connect(m_selectRole, &PageSelectRole::roleConfirmed, this, [=](const QString& roleName){
        m_currentRole = roleName;
        m_worldMap->setSelectedRole(m_currentRole);
        m_stack->setCurrentWidget(m_worldMap);
    });

    connect(m_worldMap, &PageWorldMap::requestReturnToMenu, this, [=](){
        m_stack->setCurrentWidget(m_mainMenu);
    });

    connect(m_worldMap, &PageWorldMap::requestEnterLevel, this, [=](const QString& role, const QString& levelId)
    {
        startBattle(levelId);
    });
}

void PageManager::startBattle(const QString& levelId)
{
    if(m_currentGameView != nullptr)
    {
        m_stack->removeWidget(m_currentGameView);
        m_currentGameView->deleteLater();
        m_currentGameView = nullptr;
    }
    m_currentGameView = new GameView(m_currentRole,levelId);
    m_stack->addWidget(m_currentGameView);
    m_stack->setCurrentWidget(m_currentGameView);
}