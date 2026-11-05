#ifndef PLAYER_H
#define PLAYER_H

#include <entity.h>

class Player : public Entity
{
public:
    Player(QGraphicsItem* parent = nullptr);

    void update(qreal dt)override;
    void handleInput(bool left, bool right, bool jump, bool attack);

protected:
    void updateState()override;

private:
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isJumpPressed = false;
    bool isAttackPressed = false;

    void loadPlayerAssets();
    void handleMove(qreal dt);
};

#endif // PLAYER_H
