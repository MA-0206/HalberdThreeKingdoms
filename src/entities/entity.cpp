#include "entity.h"
#include <QPainter>

Entity::Entity(QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{
    setZValue(10);
    initDebugHurt();
    initDebugAttack();
}

void Entity::tick(qreal dt)
{
    if(hitCooldownTimer > 0.0) hitCooldownTimer -= dt;
    if(knockbackTimer > 0.0) knockbackTimer -= dt;
    if(hurtStunTimer > 0.0) hurtStunTimer -= dt;

    stunManage(dt);
    updateState();
    updateAnimation();
}

// Gravity design for jumping and landing
void Entity::applyGravity(qreal dt, bool jumpPressed, qreal jumpSpeed)
{
    if(jumpPressed && (isOnGround || jumpCount < maxJumpCount))
    {
        speedY = 0.0;
        speedY = jumpSpeed;
        isOnGround = false;
        jumpCount ++;
        qDebug()<<"jumpCount:"<<jumpCount<<"maxJumpCount:"<<maxJumpCount;
    }
    speedY += gravity * dt;
    setY(y()+ speedY * dt);

    if(y()> groundY)
    {
        setY(groundY);
        speedY = 0.0;
        isOnGround = true;
        jumpCount = 0;
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
    case EntityState::Chase:  animator.play("chase"); break;
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

    updateDebugHurt();
    updateDebugAttack();
}

void Entity::takeDamage(int damage, Entity* attacker)
{
    if(m_isDead == true || hitCooldownTimer > 0)
    {
        return;
    }
    stats.runtime.hp -= damage;
    hitCooldownTimer = 0.2;
    hurtStunTimer = 0.2;

    if(attacker)
    {
        killer = attacker;
        if(x()> attacker->x())
        {
            knockbackSpeed = 100;
        }else{
            knockbackSpeed = -100;
        }
        knockbackTimer = 0.2;
    }

    if(stats.runtime.hp <= 0)
    {
        stats.runtime.hp = 0;
        m_isDead = true;
        onDeath(damage, attacker);
    }else{
        onHurt(damage, attacker);
    }
}

void Entity::stunManage(qreal dt)
{
    if(isStunned())
    {
        if(knockbackTimer > 0.0)
        {
            setX(x()+ knockbackSpeed * dt);
        }
        applyGravity(dt, false, 0);
    }else{
        update(dt);
    }
}

QRectF Entity::getHurtBox() const
{
    qreal boxWidth = frameWidth * 0.4;
    qreal boxHeight = frameHeight * 0.8;

    qreal left = x() - (boxWidth/2.0);
    qreal top = y() - frameHeight * 0.9;

    return QRectF(left, top, boxWidth, boxHeight);
}

void Entity::initDebugHurt()
{
    debugHurtBox = new QGraphicsRectItem(this);
    debugHurtBox->setPen(QPen(Qt::yellow, 2));
    debugHurtBox->setBrush(Qt::NoBrush);
    debugHurtBox->setZValue(99);
}

void Entity::initDebugAttack()
{
    debugAttackBox = new QGraphicsRectItem(this);
    debugAttackBox->setPen(QPen(Qt::red, 2));
    debugAttackBox->setBrush(Qt::NoBrush);
    debugAttackBox->setZValue(100);
    debugAttackBox->setVisible(false);
}

void Entity::updateDebugHurt()
{
    if(debugHurtBox)
    {
        QRectF worldRect = getHurtBox();
        QRectF localRect = worldRect.translated(-x(), -y());
        debugHurtBox->setRect(localRect);
    }
}

void Entity::updateDebugAttack()
{
    if(debugAttackBox)
    {
        if(isAttackActive())
        {
            debugAttackBox->setVisible(true);
            QRectF worldRect = getAttackBox();
            QRectF localRect = worldRect.translated(-x(), -y());
            if(transform().m11()< 0)
            {
                localRect.setLeft(-localRect.left());
                localRect.setRight(-localRect.right());
            }
            debugAttackBox->setRect(localRect);
        }else{
            debugAttackBox->setVisible(false);
        }
    }
}