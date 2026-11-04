#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QTimer>
#include <QElapsedTimer>
#include <QFocusEvent>
#include <QMouseEvent>
#include "animator.h"

enum class playerState{
    Idle,
    Run,
    Jump,
    Attack
};

class GameView : public QWidget
{
    Q_OBJECT
public:
    explicit GameView(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event)override;
    void keyReleaseEvent(QKeyEvent* event)override;
    void focusOutEvent(QFocusEvent* event)override;
    bool eventFilter(QObject* watched,QEvent* event)override;
    void changeEvent(QEvent* event)override;
private:
    QGraphicsScene* scene ;
    QGraphicsView* view;
    QGraphicsPixmapItem* mapItem;
    QVBoxLayout* layout;
    QGraphicsPixmapItem* player;

    QTimer* gametimer;

    playerState currentState = playerState::Idle;

    void init();
    void initSystems();
    void initViewAppearance();
    void initLayout();
    bool initLevel();
    void initPlayer();
    void initTimer();

private:
    Animator animator;

    // Elf Picture
    // QPixmap playerSpriteSheet;
    // QPixmap idleSheet;
    // QPixmap runSheet;
    // QPixmap jumpSheet;
    // QPixmap attackSheet;
    // QPixmap currentSheet;
    QElapsedTimer frameTimer;
    int frameWidth;
    int frameHeight;
    // int runFrameWidth;
    // int runFrameHeight;

    // Animation Control
    // int currentRow  = 0;
    // int currentCol = 0;
    // int animTimer = 0;

    bool isOnGround = false;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isJumpPressed = false;
    bool isAttackPressed = false;
    bool isAttacking = false;

    void loadPlayerAssets();
    void createPlayerItem();
    void upDateGame();
    void updateState();
    void handleMove(qreal dt);
    void applyGravity(qreal dt);
    void updateAnimation();
    void updateCamera();

private:
    const qreal GROUND_Y = 650;
    qreal speedY = 0.0;
    qreal gravity = 2000.0;
    qreal jumpSpeed = -800.0;

private:
    // int idleFrames = 5;
    // int runFrames = 5;
    // int jumpFrames = 4;
    // int attackFrames = 5;
    // int maxFrames = 0;

private slots:



};

#endif // GAMEVIEW_H
