#include "gameview.h"
#include <QMessageBox>
#include <qevent.h>

GameView::GameView(QWidget *parent): QWidget{parent}
{
    init();
}

void GameView::init()
{
    this->setAttribute(Qt::WA_InputMethodEnabled,false);
    initSystems();
    initViewAppearance();
    initLayout();
    if(initLevel() == false)
    {
        QMessageBox::critical(this,"ERROR","the map sources is loading failed!");
        return;
    }
    initPlayer();
}

void GameView::initSystems()
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene,this);
    layout = new QVBoxLayout(this);
    gameTimer = new QTimer(this);
    connect(gameTimer,&QTimer::timeout,this,&GameView::upDateGame);
    gameTimer->start(16);
}

// Remove the border and hide the scrollbar
void GameView::initViewAppearance()
{
    view->setStyleSheet("border:none;");
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setAlignment(Qt::AlignLeft|Qt::AlignTop);

    view->setInteractive(false);
    view->setFocusPolicy(Qt::NoFocus);
    this->setFocusPolicy(Qt::StrongFocus);
    view->installEventFilter(this);
}

// Use the layout to fill the window with the camera
void GameView::initLayout()
{
    layout->addWidget(view);
    layout->setContentsMargins(0,0,0,0);
}

// Turn the image into a physical object on stage
bool GameView::initLevel()
{
    QPixmap mapPixmap(":/assets/images/maps/level_map/level_one");
    if(mapPixmap.isNull())
    {
        qDebug()<<"the level_one map upload is failed";
        return false;
    }

    mapItem = new QGraphicsPixmapItem(mapPixmap);
    mapItem->setPos(0,0);
    mapItem->setZValue(-1);
    scene->addItem(mapItem);
    scene->setSceneRect(0,0,mapPixmap.width(),mapPixmap.height());

    return true;
}

// Create an player entity and put it in the scene
void GameView::initPlayer()
{
    CharacterConfig liubeiConfig;
    liubeiConfig.baseHp = 100;
    liubeiConfig.baseMp = 50;
    liubeiConfig.baseExp = 100;
    liubeiConfig.hpGrowth = 20;

    player = new Player(liubeiConfig);
    scene->addItem(player);
    view->centerOn(player);

    gameHud = new GameHud(this);
    initGameHud();
    gameHud->setAvatar(":/assets/images/characters/liubei/lb_avatar");
}

void GameView::upDateGame()
{
    if(!this->isActiveWindow())
    {
        isLeftPressed = false;
        isRightPressed = false;
        // return;
    }
    qreal dt = frameTimer.restart()/1000.0;
    if(dt > 0.05)
    {
        dt = 0.05;
    }
    player->handleInput(isLeftPressed, isRightPressed, isJumpPressed, isAttackPressed);
    player->update(dt);
    updateCamera();
    gameHud->updateStats(player->getStats());
}

void GameView::initGameHud()
{
    gameHud->move(10,10);
    gameHud->raise();
    gameHud->show();
    gameHud->updateStats(player->getStats());
}

void GameView::updateCamera()
{
    view->centerOn(player);
}

void GameView::keyPressEvent(QKeyEvent* event)
{
    if(event->key()== Qt::Key_A || event->key()== Qt::Key_Left)
    {
        isLeftPressed = true;
        qDebug()<<"isLeftPressed=true";
    }
    if(event->key()== Qt::Key_D || event->key()== Qt::Key_Right)
    {
        isRightPressed = true;
        qDebug()<<"isRightPressed=true";
    }
    if(event->key()== Qt::Key_K || event->key()== Qt::Key_2)
    {
        isJumpPressed = true;
        qDebug()<<"isJumpPressed = true";
    }
    if(event->key()== Qt::Key_J || event->key()== Qt::Key_1)
    {
        isAttackPressed = true;
        qDebug()<<"is AttackPressed =true";
    }
    if(event->key()== Qt::Key_H)
        player->takeDamage(10);
    if(event->key()== Qt::Key_B)
        player->heal(20);
}

void GameView::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key()== Qt::Key_A || event->key()== Qt::Key_Left)
    {
        isLeftPressed = false;
        qDebug()<<"isLeftPressed = false";
    }
    if(event->key()== Qt::Key_D || event->key()== Qt::Key_Right)
    {
        isRightPressed = false;
        qDebug()<<"isRightPressed = false";

    }
    if(event->key()== Qt::Key_K || event->key()== Qt::Key_2)
    {
        isJumpPressed = false;
        qDebug()<<"isJumpPressed is false";
    }
    if(event->key()== Qt::Key_J || event->key()== Qt::Key_1)
    {
        isAttackPressed = false;
        qDebug()<<"is AttackPressed = false";
    }
}

// Window focus loss handling
void GameView::focusOutEvent(QFocusEvent* event)
{
    isLeftPressed = false;
    isRightPressed = false;

    QWidget::focusOutEvent(event);
}

void GameView::changeEvent(QEvent* event)
{
    if(event->type()== QEvent::ActivationChange)
    {
        if(this->isActiveWindow())
        {
            this->setFocus();
        }
    }
    QWidget::changeEvent(event);
}

bool GameView::eventFilter(QObject* watched,QEvent* event)
{
    if(watched == view && event->type()== QEvent::MouseButtonPress)
    {
        this->setFocus();
    }

    return QWidget::eventFilter(watched,event);
}
