#include "entity.h"

Entity::Entity(QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{
    setZValue(10);
}

// Gravity design for jumping and landing
void Entity::applyGravity(qreal dt, bool jumpPressed, qreal jumpSpeed)
{
    if(jumpPressed && isOnGround)
    {
        speedY = jumpSpeed;
        isOnGround = false;
    }
    speedY += gravity * dt;
    setY(y()+ speedY * dt);

    if(y()> groundY)
    {
        setY(groundY);
        speedY = 0.0;
        isOnGround = true;
    }
}

// Update character frame animation
void Entity::updateAnimation()
{
    switch(currentState)
    {
    case EntityState::Idle:   animator.play("idle"); break;
    case EntityState::Run:    animator.play("run"); break;
    case EntityState::Jump:   animator.play("jump"); break;
    case EntityState::Attack: animator.play("attack"); break;
    case EntityState::Hurt:   animator.play("hurt"); break;
    case EntityState::Dead:   animator.play("dead"); break;
    default: break;
    }

    animator.update();
    QPixmap currentPix = animator.getCurrentPixmap();
    if(!currentPix.isNull())
    {
        setPixmap(currentPix);
    }
}