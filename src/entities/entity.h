#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include "Animator.h"
#include "EntityState.h"


struct BaseAttributes {
    int maxHp = 100;
    int maxMp = 50;
    int maxExp = 100;
};

struct RunTimeState {
    int hp = 100;
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

    virtual void update(qreal dt)= 0;

    const EntityStats& getStats()const {return stats;}
protected:
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

    virtual void updateState() = 0;

};

#endif // ENTITY_H
