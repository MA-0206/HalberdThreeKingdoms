#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include "Animator.h"
#include "EntityState.h"


struct BaseAttributes {
    int maxHp = 1000;
    int maxMp = 50;
    int maxExp = 100;
    int maxLevel = 80;
};

struct RunTimeState {
    int hp = 1000;
    int mp = 50;
    int exp = 0;
    int level = 1;
};

struct EntityStats {
    BaseAttributes base;
    RunTimeState runtime;
};

class Entity : public QGraphicsPixmapItem
{
public:
    Entity(QGraphicsItem* parents = nullptr);
    virtual ~Entity() = default;

    const EntityStats& getStats()const {return stats;}
    void tick(qreal dt);
    virtual void update(qreal dt)= 0;

    void takeDamage(int damage, Entity* attacker = nullptr);
    bool isDead() const {return m_isDead;}
    bool canBeRemoved() const {return m_isDead && animator.isFinshed();}

    virtual bool isAttackActive() const {return false;}
    virtual QRectF getAttackBox() const {return QRectF();}
    QRectF getHurtBox() const;

protected:
    virtual void updateState() = 0;
    virtual void onHurt(){}
    virtual void onDeath(){}

    EntityStats stats;
    // General components and states
    Animator animator;
    EntityState currentState = EntityState::Idle;
    int frameWidth = 0;
    int frameHeight = 0;

    // General physical variables
    qreal speedY = 0.0;
    qreal gravity = 2000.0;
    qreal groundY = 650.0;
    bool isOnGround = false;
    void applyGravity(qreal dt, bool JumpPressed, qreal jumpSpeed);
    void updateAnimation();

    bool m_isDead = false;
    qreal hitCooldownTimer = 0.0;
    qreal hurtStunTimer = 0.0;
    qreal knockbackTimer = 0.0;
    qreal knockbackSpeed = 0.0;
    bool isStunned() const {return hurtStunTimer > 0.0;}
    void stunManage(qreal dt);

    QGraphicsRectItem* debugHurtBox = nullptr;
    QGraphicsRectItem* debugAttackBox = nullptr;
    void initDebugHurt();
    void initDebugAttack();
    void updateDebugHurt();
    void updateDebugAttack();
};

#endif // ENTITY_H
