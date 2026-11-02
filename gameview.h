#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QTimer>
#include <QElapsedTimer>
class GameView : public QWidget
{
    Q_OBJECT
public:
    explicit GameView(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event)override;
    void keyReleaseEvent(QKeyEvent* event)override;

private:
    QGraphicsScene* scene ;
    QGraphicsView* view;
    QGraphicsPixmapItem* mapItem;
    QVBoxLayout* layout;
    QGraphicsPixmapItem* player;

    QTimer* gametimer;

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

    bool isLeftPressed = false;
    bool isRightPressed = false;

    void loadPlayerAssets();
    void createPlayerItem();
    void upDateGame();
    void handleInput();
    void updateAnimation();
    void updateCamera();


private slots:



};

#endif // GAMEVIEW_H
