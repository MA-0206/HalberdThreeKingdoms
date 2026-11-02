#include "gameview.h"

#include <QMessageBox>
#include <qevent.h>

GameView::GameView(QWidget *parent): QWidget{parent}
{
    init();
}

void GameView::init()
{
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
    view->setFocusPolicy(Qt::StrongFocus);
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
    QPixmap mapPixmap(":/level_one");
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
    if(event->key()== Qt::Key_Left || event->key()== Qt::Key_A)
    {
        isLeftPressed = true;
    }
    if(event->key()== Qt::Key_Right || event->key()== Qt::Key_D)
    {
        isRightPressed = true;
    }
}

void GameView::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key()== Qt::Key_Left || event->key()== Qt::Key_A)
    {
        isLeftPressed = false;
    }
    if(event->key()== Qt::Key_Right || event->key()== Qt::Key_D)
    {
        isRightPressed = false;
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
    playerSpriteSheet.load(":/liubei");
    if(playerSpriteSheet.isNull())
    {
        qDebug()<<"the player of liubei  is loading failed!";
        return;
    }

    FrameWidth = playerSpriteSheet.width()/4;
    FrameHeight = playerSpriteSheet.height()/4;
}

// Create an player entity and put it in the scene
void GameView::createPlayerItem()
{
    if(playerSpriteSheet.isNull())
    {
        return;
    }

    QPixmap firstFrame = playerSpriteSheet.copy(0,0,FrameWidth,FrameHeight);
    player = new QGraphicsPixmapItem(firstFrame);

    player->setScale(0.4);
    player->setOffset(-FrameWidth/2,-FrameHeight);
    player->setPos(500,650);
    player->setZValue(10);

    scene->addItem(player);
    view->centerOn(player);
}

void GameView::upDateGame()
{
    handleInput();
    updateAnimation();
    updateCamera();
}

void GameView::handleInput()
{
    qreal dt = frameTimer.restart()/1000.0;
    qreal speedPerSecond = 300.0;
    qreal moveDistance = speedPerSecond*dt;

    bool isMoving = false;

    if(isRightPressed)
    {
        player->setX(player->x() + moveDistance);
        isMoving = true;
        player->setTransform(QTransform());
    }
    if(isLeftPressed)
    {
        player->setX(player->x() - moveDistance);
        isMoving = true;
        player->setTransform(QTransform::fromScale(-1,1));
    }
    currentRow = isMoving? 1 : 0 ;
}

void GameView::updateAnimation()
{
    animTimer++;
    if(animTimer >= 6)
    {
        animTimer = 0;

        currentCol++;
        if(currentCol>3)
        {
            currentCol = 0;
        }

        int x = currentCol * FrameWidth;
        int y = currentRow * FrameHeight;

        player->setPixmap(playerSpriteSheet.copy(x,y,FrameWidth,FrameHeight));
    }

}

void GameView::updateCamera()
{
    view->centerOn(player);
}