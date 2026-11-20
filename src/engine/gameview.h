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
#include "world.h"
#include "gamehud.h"

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
    void initWorld();
    void initGameHud();

private:
    World* world;
    GameHud* gameHud;
    QElapsedTimer frameTimer;

    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isJumpPressed = false;
    bool isAttackPressed = false;

    qreal m_lastCamMaxX = 0.0;
    qreal m_smoothTimer = 0.0;
    bool m_isSmoothCamera = false;

    // The smaller the slope, the faster one goes down.
    const qreal SMOOTH_SPEED = -8.0;

    void createPlayerItem();
    void upDateGame();
    void updateCamera(qreal dt);
};

#endif // GAMEVIEW_H
