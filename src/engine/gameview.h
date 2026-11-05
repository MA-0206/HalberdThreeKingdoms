#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QTimer>
#include <QElapsedTimer>
#include <QFocusEvent>
#include <QMouseEvent>
#include "animator.h"
#include "player.h"

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
    QTimer* gameTimer;

    void init();
    void initSystems();
    void initViewAppearance();
    void initLayout();
    bool initLevel();
    void initPlayer();

private:
    Animator animator;
    Player *player;
    QElapsedTimer frameTimer;

    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isJumpPressed = false;
    bool isAttackPressed = false;

    void createPlayerItem();
    void upDateGame();
    void updateCamera();
};

#endif // GAMEVIEW_H
