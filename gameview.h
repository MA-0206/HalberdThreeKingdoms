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
    // Elf Picture
    QPixmap playerSpriteSheet;
    QElapsedTimer frameTimer;
    int FrameWidth;
    int FrameHeight;

    // Animation Control
    int currentRow  = 0;
    int currentCol = 0;
    int animTimer = 0;

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


private slots:



};

#endif // GAMEVIEW_H
