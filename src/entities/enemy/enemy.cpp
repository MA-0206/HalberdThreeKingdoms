#include "enemy.h"

Enemy::Enemy(const EnemyConfig& config, QGraphicsItem* parent)
    :Entity(parent),myConfig(config)
{
    faction = Faction::Enemy;
    loadEnemyAssets();
    initStats();
}

void Enemy::update(qreal dt)
{
    applyGravity(dt,false,0);
    handleAction(dt);
}

// Load the animation of the minor monster
void Enemy::loadEnemyAssets()
{
    QString path = ":/assets/images/enemies/" + myConfig.name + "/";
    animator.addAnimation("idle",path+"ys_idle",9,2,5,5);
    animator.addAnimation("run",path+"ys_run",7,2,4,6);
    animator.addAnimation("chase",path+"ys_run",7,2,4,4);
    animator.addAnimation("hurt",path+"ys_hurt",10,2,5,4);
    animator.addAnimation("dead",path+"ys_dead",14,2,7,3);
    animator.addAnimation("attack",path+"ys_attack",12,2,6,4);

    animator.play("idle");
    frameWidth = animator.getFrameWidth();
    frameHeight = animator.getFrameHeight();

    if(frameWidth > 0 && frameHeight > 0)
    {
        setOffset(-frameWidth/2, -frameHeight);
    }
    setPos(400, groundY);
}

void Enemy::initStats()
{
    stats.base.maxHp = myConfig.maxHp;

    stats.runtime.hp = stats.base.maxHp;

    patrolCenterX = x();
}

void Enemy::handleAction(qreal dt)
{
    if(currentState == EntityState::Idle)
    {
        waitTimer += dt;
        if (!hasValidTarget() || !isTargetInSightRange())
        {
            lostTimer += dt;
        }
    }
    if(hitCoolTimer > 0)
    {
        hitCoolTimer -= dt;
    }
    if(currentState == EntityState::Chase)
    {
        updateChase(dt);
    }
    if(currentState == EntityState::Run)
    {
        updatePatrol(dt);
    }
}

void Enemy::setTarget(Entity* target)
{
    if(m_target != target)
    {
        m_target = target;
        lostTimer = 0.0;
    }
}

// The mechanism of small monsters' patrol
void Enemy::updatePatrol(qreal dt)
{
    if(!isOnGround) return;
    // qDebug() << "正在巡逻... 速度:" << myConfig.runSpeed << " 当前位置:" << x() << " 方向:" << moveDirection;
    if(moveDirection == 1 && transform().m11() < 0)
    {
        setTransform(QTransform());
    }else if(moveDirection == -1 && transform().m11() > 0)
    {
        setTransform(QTransform::fromScale(-1,1));
    }

    qreal moveDistance = myConfig.runSpeed * dt;
    setX(x()+ (moveDistance * moveDirection));

    qreal patrolMaxX = patrolCenterX + myConfig.patrolRange;
    qreal patrolMinX = patrolCenterX - myConfig.patrolRange;

    if(x()>= patrolMaxX && moveDirection == 1)
    {
        isWaitingAtEdge = true;
    }else if(x()<= patrolMinX && moveDirection == -1)
    {
        isWaitingAtEdge = true;
    }
}

void Enemy::handleEdgeWaitTimeout()
{
    if(waitTimer >= waitDuration)
    {
        isWaitingAtEdge = false;
        waitTimer = 0;

        moveDirection *= -1;
        if(moveDirection == 1)
        {
            setTransform(QTransform());
        }else{
            setTransform(QTransform::fromScale(-1,1));
        }
        currentState = EntityState::Run;
    }
}

void Enemy::handleLostWaitTimeout()
{
    if(lostTimer >= myConfig.lostTargetWaitTime)
    {
        currentState = EntityState::Run;

        patrolCenterX = x();
        isWaitingAtEdge = false;
        moveDirection = -1;
        lostTimer = 0;
    }
}

void Enemy::updateChase(qreal dt)
{
    if(!hasValidTarget() || !isTargetInSightRange())
    {
        return;
    }
    if(std::abs(m_target->x() - x()) < 5)
        return;
    qreal moveDistance = myConfig.chaseSpeed * dt;
    if(m_target->x() > x())
    {
        setX(x() + moveDistance);
        setTransform(QTransform());
    }else{
        setX(x() - moveDistance);
        setTransform(QTransform::fromScale(-1, 1));
    }
}

QRectF Enemy::getAttackBox() const
{
    qreal atkWidth = animator.getFrameWidth()* 0.4;
    qreal atkHeight = animator.getFrameHeight()* 0.6;
    qreal left;
    qreal top = y() - animator.getFrameHeight()* 0.8;

    qreal offSetX = animator.getFrameWidth()* 0.1;
    if(transform().m11() > 0)
    {
        left = x() + offSetX;
    }else{
        left = x() - offSetX - atkWidth;
    }

    return QRectF(left, top, atkWidth, atkHeight);
}

void Enemy::onHurt(int damage, Entity* attacker)
{
    currentState = EntityState::Hurt;
    qDebug()<<"the"<<myConfig.name <<"was hitted by"
    <<attacker->getName()<<"damage is"<< damage <<"Remaining hp:"<< stats.runtime.hp;
}

void Enemy::onDeath(int damage, Entity* killer)
{
    currentState = EntityState::Dead;
    qDebug()<<"the "<<myConfig.name<<"was"<< killer->getName()<<"killed!"<< "the Fatal damage:"<<damage;
}

bool Enemy::isAttackActive() const
{
    return (currentState == EntityState::Attack && animator.getCurrentFrame() == 1);
}

bool Enemy::hasValidTarget() const
{
    if(m_target == nullptr)
    {
        return false;
    }
    if(m_target->isDead() || !canAttack(faction, m_target->faction))
    {
        return false;
    }
    return true;
}

bool Enemy::isTargetInAttackRange() const
{   if(m_target == nullptr)
    {
        return false;
    }
    return std::abs(m_target->x()- x()) <= myConfig.attackRange;
}

bool Enemy::isTargetInSightRange() const
{
    if(m_target == nullptr)
    {
        return false;
    }
    return std::abs(m_target->x() - x()) <= myConfig.sightRange;
}

// Enemy State Machine
void Enemy::updateState()
{
    // qDebug() << "当前状态:" << (int)currentState << " 目标:" << m_target;
    switch(currentState)
    {
    case EntityState::Dead:
        return;
        break;
    case EntityState::Hurt:
        if(hurtStunTimer <= 0)
        {
            if(m_target == nullptr)
            {
                currentState = EntityState::Idle;
            }else{
                currentState = EntityState::Chase;
            }
        }else{
            return;
        }
        break;
    case EntityState::Attack:
        if(animator.isFinshed())
        {
            currentState = EntityState::Idle;
        }
        return;
        break;
    case EntityState::Chase:
        lostTimer = 0.0;
        if(isOnGround)
        {
            if(isTargetInSightRange())
            {
                if(isTargetInAttackRange() && hitCoolTimer <= 0)
                {
                        currentState = EntityState::Attack;
                        hitCoolTimer = myConfig.attackCD;
                }
                return;
            }else{
                currentState = EntityState::Idle;
            }
        }else{
            currentState = EntityState::Jump;
        }
        break;
    case EntityState::Jump:
        if(isOnGround)
        {
            if(isTargetInSightRange())
            {
                currentState = EntityState::Chase;
            }else if(!isWaitingAtEdge)
            {
                currentState = EntityState::Run;
            }else{
                currentState = EntityState::Idle;
            }
        }
        break;
    case EntityState::Idle:
        if(isOnGround)
        {
            if(hasValidTarget() && isTargetInSightRange())
            {
                currentState = EntityState::Chase;
                break;
            }
            if(isWaitingAtEdge)
            {
                handleEdgeWaitTimeout();
            }else{
                handleLostWaitTimeout();
            }
        }
        else{
            currentState = EntityState::Jump;
        }
        break;
    case EntityState::Run:
        waitTimer = 0.0;
        if(isOnGround)
        {
            if(isTargetInSightRange())
            {
                currentState = EntityState::Chase;
            }
            else if(isWaitingAtEdge)
            {
                currentState = EntityState::Idle;
            }
        }
        else{
            currentState = EntityState::Jump;
        }
        break;
    }
}