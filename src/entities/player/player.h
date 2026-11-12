#ifndef PLAYER_H
#define PLAYER_H

#include <entity.h>


struct CharacterConfig{
    QString name = "liubei";
    int baseHp = 100;
    int baseMp = 50;
    int baseExp = 100;
    int force = 30;
    int hpGrowth = 100;
    int mpGrowth = 50;
    qreal moveSpeed = 300.0;
    qreal jumpSpeed = -800.0;
};

class Player : public Entity
{
public:
    Player(CharacterConfig& config, QGraphicsItem* parent = nullptr);

    void update(qreal dt) override;
    // bool isAttackActive() const override;
    bool isAttackActive() const override;
    QRectF getAttackBox() const override;
    void handleInput(bool left, bool right, bool jump, bool attack);
    // void takeDamage(int damage);
    void heal(int amount);
    void replayMagic(int magic);
    void deleteMagic(int magic);
    void addExp(int exp);


protected:
    void updateState() override;
    void onHurt(int damage, Entity* attacker) override;
    void onDeath(int damage, Entity* attacker) override;

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
