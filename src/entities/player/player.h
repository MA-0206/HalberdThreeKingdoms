#ifndef PLAYER_H
#define PLAYER_H

#include <entity.h>


struct CharacterConfig{
    int baseHp = 100;
    int baseMp = 50;
    int baseExp = 100;
    int hpGrowth = 100;
    int mpGrowth = 50;
    qreal moveSpeed = 300.0;
    qreal jumpSpeed = -800.0;
};

class Player : public Entity
{
public:
    Player(CharacterConfig& config, QGraphicsItem* parent = nullptr);

    void update(qreal dt)override;
    void handleInput(bool left, bool right, bool jump, bool attack);
    void takeDamage(int damage);
    void heal(int amount);

protected:
    void updateState()override;

private:
    CharacterConfig myConfig;
    bool isLeftPressed = false;
    bool isRightPressed = false;
    bool isJumpPressed = false;
    bool isAttackPressed = false;

    void loadPlayerAssets();
    void handleMove(qreal dt);
    void initStats();
};

#endif // PLAYER_H
