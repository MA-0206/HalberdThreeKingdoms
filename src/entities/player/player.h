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
    QRectF getHurtBox() const override;
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
    bool isAttackBuffered = false;
    bool lastAttackPressed = false;
    qreal comboWindowTimer = 0.0f;
    const qreal COMBO_WINDOW_TIME = 0.5f;

    void loadPlayerAssets();
    void initStats();
    void resetAttack(qreal dt);
    void startAttack();
    void handleAttackEnd();
    void handleMove(qreal dt);
    void clampRolePos();
    void advanceAttack(qreal dt);
    void upgradeState();
    void updateInputDebounce();
};

#endif // PLAYER_H
