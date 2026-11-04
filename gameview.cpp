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
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&GameView::upDateGame);
    timer->start(16);
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

void GameView::initPlayer()
{
    loadPlayerAssets();
    createPlayerItem();
}

// Load the sprite sheet and calculate the size
void GameView::loadPlayerAssets()
{
    animator.addAnimation("lb_idle",QPixmap(":/assets/images/characters/liubei/lb_idle"),18,2,9,6);
    animator.addAnimation("lb_run",QPixmap(":/assets/images/characters/liubei/lb_run"),5,1,5,3);
    animator.addAnimation("lb_jump",QPixmap(":/assets/images/characters/liubei/lb_jump"),23,3,8,4);
    animator.play("lb_idle");

    frameWidth = animator.getCurrentPixmap().width();
    frameHeight = animator.getCurrentPixmap().height();
}

// Create an player entity and put it in the scene
void GameView::createPlayerItem()
{
    QPixmap firstFrame = animator.getCurrentPixmap();
    if(firstFrame.isNull())
    {
        return;
    }
    int frameH = animator.getFrameHeight();
    int frameW = animator.getFrameWidth();

    player = new QGraphicsPixmapItem(firstFrame);
    player->setScale(1.0);
    player->setOffset(-frameW/2,-frameH);
    player->setPos(500,650);
    player->setZValue(10);

    scene->addItem(player);
    view->centerOn(player);
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
    handleMove(dt);
    applyGravity(dt);
    updateState();
    updateAnimation();
    updateCamera();
}

void GameView::handleMove(qreal dt)
{

    qreal speedPerSecond = 300.0;
    qreal moveDistance = speedPerSecond*dt;

    if(isRightPressed)
    {
        player->setX(player->x() + moveDistance);
        player->setTransform(QTransform());
    }
    if(isLeftPressed)
    {
        player->setX(player->x() - moveDistance);
        player->setTransform(QTransform::fromScale(-1,1));
    }
}

void GameView::updateState()
{
    if(isAttackPressed && currentState != playerState::Attack)
    {
        currentState = playerState::Attack;
        return;
    }
    switch(currentState)
    {
    case playerState::Attack:
        if(animator.isFinshed())
        {
            if(!isOnGround)
            {
                currentState = playerState::Jump;
            }
            else if(isLeftPressed || isRightPressed)
            {
                currentState = playerState::Run;
            }
            else{
                currentState = playerState::Idle;
            }
        }
        break;
    case playerState::Idle:
        if(!isOnGround)
        {
            currentState = playerState::Jump;
        }
        else if(isLeftPressed || isRightPressed)
        {
            currentState = playerState::Run;
        }
        break;

    case playerState::Run:
        if(!isOnGround)
        {
            currentState = playerState::Jump;
        }
        else if(!isLeftPressed && !isRightPressed)
        {
            currentState = playerState::Idle;
        }
        break;
    case playerState::Jump:
        if(isOnGround)
        {
            if(isLeftPressed || isRightPressed)
            {
                currentState = playerState::Run;
            }
            else{
                currentState = playerState::Idle;
            }
        }
        break;
    }
}

// Gravity design for jumping and landing
void GameView::applyGravity(qreal dt)
{
    if(isJumpPressed && isOnGround)
    {
        speedY = jumpSpeed;
        isOnGround = false;
    }
    speedY += gravity * dt;
    player->setY(player->y()+ speedY * dt);

    if(player->y()>GROUND_Y)
    {
        player->setY(GROUND_Y);
        speedY = 0;
        isOnGround = true;
    }
}

// Update character frame animation
void GameView::updateAnimation()
{
    switch(currentState)
    {
    case playerState::Idle: animator.play("lb_idle"); break;
    case playerState::Run: animator.play("lb_run"); break;
    case playerState::Jump: animator.play("lb_jump"); break;
    case playerState::Attack: animator.play("lb_attack"); break;
    }
    animator.update();
        if(currentState == playerState::Attack)
        {
            isAttacking = false;
        }
        QPixmap currentPix = animator.getCurrentPixmap();
        if(!currentPix.isNull())
        {
            player->setPixmap(currentPix);
        }
}

void GameView::updateCamera()
{
    view->centerOn(player);
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
