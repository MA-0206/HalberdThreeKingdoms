#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include "Animator.h"
#include "EntityData.h"

class Entity : public QGraphicsPixmapItem
{
public:
    Entity(QGraphicsItem* parents = nullptr);
    virtual ~Entity() = default;

    Entity* killer;
    Faction faction = Faction::Neutral;

    virtual void update(qreal dt)= 0;

    const EntityStats& getStats()const {return stats;}
    QString getName() const {return stats.runtime.name;}
    bool isDead() const {return m_isDead;}
    bool isStunned() const {return hurtStunTimer > 0.0;}
    bool canBeRemoved() const {return m_isDead && animator.isFinshed();}
    virtual bool isAttackActive() const {return false;}
    virtual QRectF getAttackBox() const {return QRectF();}

    void tick(qreal dt);
    void takeDamage(int damage, Entity* attacker = nullptr);
    QRectF getHurtBox() const; 
protected:
    virtual void updateState() = 0;
    virtual void onHurt(int damage, Entity* attacker){}
    virtual void onDeath(int damage, Entity* attacker){}

    void applyGravity(qreal dt, bool JumpPressed, qreal jumpSpeed);
    void updateAnimation();
    void stunManage(qreal dt);
    void initDebugHurt();
    void initDebugAttack();
    void updateDebugHurt();
    void updateDebugAttack();

    EntityStats stats;
    Animator animator;
    EntityState currentState = EntityState::Idle;
    int frameWidth = 0;
    int frameHeight = 0;
    qreal speedY = 0.0;
    qreal gravity = 2000.0;
    qreal groundY = 650.0;
    qreal hitCooldownTimer = 0.0;
    qreal hurtStunTimer = 0.0;
    qreal knockbackTimer = 0.0;
    qreal knockbackSpeed = 0.0;
    bool isOnGround = false;
    bool m_isDead = false;

    QGraphicsRectItem* debugHurtBox = nullptr;
    QGraphicsRectItem* debugAttackBox = nullptr;
};

#endif // ENTITY_H
