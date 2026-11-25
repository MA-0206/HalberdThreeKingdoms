#include "gameview.h"
#include "jsonparser.h"
#include <QMessageBox>
#include <qevent.h>

GameView::GameView(const QString& characterId, const QString& levelId,
                   QWidget *parent): QWidget{parent}, m_levelId(levelId)
{
    init(characterId, levelId);

}

void GameView::init(const QString& characterId, const QString& levelId)
{
    JsonParser::getInstance().loadAllConfigs();
    this->setAttribute(Qt::WA_InputMethodEnabled,false);
    initSystems();
    initViewAppearance();
    initLayout();
    initWorld(characterId,levelId);
    if(initLevel() == false)
    {
        QMessageBox::critical(this,"ERROR","the map sources is loading failed!");
        return;
    }
}

// Initialization interface resources
void GameView::initSystems()
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene,this);
    layout = new QVBoxLayout(this);
    gameTimer = new QTimer(this);
    connect(gameTimer,&QTimer::timeout,this,&GameView::upDateGame);
    // gameTimer->start(16);
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
    QString mapPath = world->getLevelMapPath();
    if(mapPath.isEmpty())
    {
        qDebug()<<"the levelMapPath is error,please check the level map path";
        return false;
    }
    QPixmap mapPixmap(mapPath);
    if(mapPixmap.isNull())
    {
        qDebug()<<"the level map image is loading failed";
        return false;
    }

    mapItem = new QGraphicsPixmapItem(mapPixmap);
    mapItem->setPos(0,0);
    mapItem->setZValue(-1);
    scene->addItem(mapItem);
    scene->setSceneRect(0,0,mapPixmap.width(),mapPixmap.height());
    return true;
}

// Initialize the world and render
void GameView::initWorld(const QString& characterId, const QString& levelId)
{
    world = new World(characterId, levelId);
    scene->addItem(world->getPlayer());
    view->centerOn(world->getPlayer());

    gameHud = new GameHud(this);
    initGameHud();
    gameHud->setAvatar(world->getPlayer()->getConfig().avatarPath);
}

void GameView::upDateGame()
{
    if(!this->isActiveWindow())
    {
        isLeftPressed = false;
        isRightPressed = false;
    }
    qreal dt = frameTimer.restart()/1000.0;
    if(dt > 0.05)
    {
        dt = 0.05;
    }

    world->update(dt, isLeftPressed, isRightPressed, isJumpPressed, isAttackPressed);
    auto& newEnemies = world->getPendingEnemies();
    for(Enemy* e:newEnemies)
    {
        scene->addItem(e);
    }
    world->clearPendingEnemies();
    updateCamera(dt);
    gameHud->updateStats(world->getPlayer()->getStats());
}

// Initialize the health bar system
void GameView::initGameHud()
{
    gameHud->move(10,10);
    gameHud->raise();
    gameHud->show();
    gameHud->updateStats(world->getPlayer()->getStats());
}

void GameView::startGame()
{
    if(!gameTimer->isActive())
    {
        frameTimer.restart();
        gameTimer->start(16);
    }
}

void GameView::updateCamera(qreal dt)
{
    qreal targetX = world->getPlayer()->x();
    qreal halfScreen = view->width()/2;

    qreal camMinX = world->getCameraMinX();
    qreal camMaxX = world->getCameraMaxX();

    qreal minCenter = camMinX + halfScreen;
    qreal maxCenter = camMaxX - halfScreen;

    if(minCenter > maxCenter)
    {
        targetX = (minCenter + maxCenter)/2.0;
    }else{
        targetX = std::clamp(targetX, minCenter, maxCenter);
    }

    if(m_lastCamMaxX < camMaxX)
    {
        m_isSmoothCamera = true;
        m_smoothTimer = 0.0;
    }
    m_lastCamMaxX = camMaxX;

    if(m_isSmoothCamera)
    {
        m_smoothTimer += dt;
        if(m_smoothTimer > 0.8)
        {
            m_isSmoothCamera = false;
            view->centerOn(targetX,world->getPlayer()->y());
        }else{
            qreal currentCamX = view->mapToScene(view->viewport()->rect().center()).x();
            qreal lerpFactor = 1.0 - std::exp(SMOOTH_SPEED * dt);
            qreal nextX = currentCamX +  (targetX - currentCamX) * lerpFactor;
            view->centerOn(nextX, world->getPlayer()->y());
        }
    }else{
    view->centerOn(targetX, world->getPlayer()->y());
    }
}

void GameView::keyPressEvent(QKeyEvent* event)
{
    if(event->key()== Qt::Key_A || event->key()== Qt::Key_Left)
    {
        isLeftPressed = true;
        // qDebug()<<"isLeftPressed=true";
    }
    if(event->key()== Qt::Key_D || event->key()== Qt::Key_Right)
    {
        isRightPressed = true;
        // qDebug()<<"isRightPressed=true";
    }
    if(event->key()== Qt::Key_K || event->key()== Qt::Key_2)
    {
        isJumpPressed = true;
        // qDebug()<<"isJumpPressed = true";
    }
    if(event->key()== Qt::Key_J || event->key()== Qt::Key_1)
    {
        isAttackPressed = true;
        // qDebug()<<"is AttackPressed =true";
    }
    if(event->key()== Qt::Key_H)
        world->getPlayer()->takeDamage(10, world->getPlayer());
    if(event->key()== Qt::Key_B)
        world->getPlayer()->deleteMagic(20);
}

void GameView::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key()== Qt::Key_A || event->key()== Qt::Key_Left)
    {
        isLeftPressed = false;
        // qDebug()<<"isLeftPressed = false";
    }
    if(event->key()== Qt::Key_D || event->key()== Qt::Key_Right)
    {
        isRightPressed = false;
        // qDebug()<<"isRightPressed = false";

    }
    if(event->key()== Qt::Key_K || event->key()== Qt::Key_2)
    {
        isJumpPressed = false;
        // qDebug()<<"isJumpPressed is false";
    }
    if(event->key()== Qt::Key_J || event->key()== Qt::Key_1)
    {
        isAttackPressed = false;
        // qDebug()<<"is AttackPressed = false";
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
