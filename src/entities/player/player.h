#ifndef PLAYER_H
#define PLAYER_H

#include <entity.h>

struct AnimationConfig{
    int totalFrames;
    int rows;
    int cols;
    int tickRate;
};

struct CharacterConfig{
    QString name;
    QString avatarPath;
    qreal baseHp;
    qreal baseMp;
    qreal baseExp;
    qreal hpGrowth;
    qreal hpRegenPerSec;
    qreal mpGrowth;
    qreal expGrowth;
    qreal moveSpeed;
    qreal jumpSpeed;
    int baseAttack;
    int attackGrowth;
    std::map<QString,AnimationConfig> animations;
};

class Player : public Entity
{
public:
    Player(CharacterConfig& config, QGraphicsItem* parent = nullptr);

    const CharacterConfig& getConfig() const {return myConfig;}
    void update(qreal dt) override;
    bool isAttackActive() const override;
    QRectF getAttackBox() const override;
    void handleInput(bool left, bool right, bool jump, bool attack);
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
    bool wasJumpPressed = false;
    bool jumpTrigger = false;

    void loadPlayerAssets();
    void handleMove(qreal dt);
    void initStats();
    void upgradeState();
    void updateInputDebounce();
};

#endif // PLAYER_H
