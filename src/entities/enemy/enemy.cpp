#include "enemy.h"

Enemy::Enemy(const EnemyConfig& config, QGraphicsItem* parent)
    :Entity(parent),myConfig(config)
{
    loadEnemyAssets();
}

void Enemy::update(qreal dt)
{
    applyGravity(dt,false,0);
    updatePatrol(dt);
    updateState();
    updateAnimation();
}

// Load the animation of the minor monster
void Enemy::loadEnemyAssets()
{
    QString path = ":/assets/images/enemies/" + myConfig.spriteFolder + "/";
    animator.addAnimation("idle",path+"ys_idle",9,2,5,5);
    animator.addAnimation("run",path+"ys_run",7,2,4,6);

    animator.play("idle");
    frameWidth = animator.getFrameWidth();
    frameHeight = animator.getFrameHeight();

    if(frameWidth > 0 && frameHeight > 0)
    {
        setOffset(-frameWidth/2, -frameHeight);
    }
    setPos(400, groundY);
}

// The mechanism of small monsters' patrol
void Enemy::updatePatrol(qreal dt)
{
    if(!isOnGround)
    {
        return;
    }
    if(isWaitingAtEdge)
    {
        waitTimer += dt;
        if(waitTimer >= waitDuration)
        {
            isWaitingAtEdge = false;
            waitTimer = 0;

            moveDirection *= -1;

            if(moveDirection == 1)
            {
                setTransform(QTransform());
            }
            else{
                setTransform(QTransform::fromScale(-1, 1));
            }
        }
    }else
    {
        qreal moveDistance = myConfig.runSpeed * dt;
        setX(x()+ (moveDistance * moveDirection));

        if(x()>= myConfig.patrolMaxX && moveDirection == 1)
        {
            isWaitingAtEdge = true;
            waitTimer = 0.0;
        }else if(x()<= myConfig.patrolMinX && moveDirection == -1)
        {
            isWaitingAtEdge = true;
            waitTimer = 0.0;
        }
    }
}

// Enemy State Machine
void Enemy::updateState()
{
    switch(currentState)
    {
    case EntityState::Jump:
        if(isOnGround)
        {
            if(isWaitingAtEdge)
            {
                currentState = EntityState::Idle;
            }
            else{
                currentState = EntityState::Run;
            }
        }
        break;
    case EntityState::Idle:
        if(isOnGround)
        {
            if(!isWaitingAtEdge)
            {
                currentState = EntityState::Run;
            }
        }
        else{
            currentState = EntityState::Jump;
        }
        break;
    case EntityState::Run:
        if(!isOnGround)
        {
            currentState = EntityState::Jump;
        }
        else if(isWaitingAtEdge)
        {
            currentState = EntityState::Idle;
        }
        break;
    }
}