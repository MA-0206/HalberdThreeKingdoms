#include "player.h"

Player::Player(CharacterConfig& config, QGraphicsItem* parent):Entity(parent),myConfig(config)
{
    faction = Faction::Player;
    initStats();
    loadPlayerAssets();
    setZValue(11);
}

// Initialize character status
void Player::initStats()
{
    maxJumpCount = 2;
    maxComboCount = 3;
    stats.base.name   = myConfig.name;
    stats.base.maxHp  = myConfig.baseHp;
    stats.base.maxMp  = myConfig.baseMp;
    stats.base.maxExp = myConfig.baseExp;

    stats.runtime.name = stats.base.name;
    stats.runtime.hp   = stats.base.maxHp;
    stats.runtime.mp   = stats.base.maxMp;
}

// Load the sprite sheet and calculate the size
void Player::loadPlayerAssets()
{
    QString path = ":/assets/images/characters/" + myConfig.name + "/";
    for(const auto& pair:myConfig.animations)
    {
        QString animName = pair.first;
        const AnimationConfig& cfg = pair.second;
        animator.addAnimation(animName ,QPixmap(path + animName), cfg.totalFrames,
                              cfg.rows, cfg.cols, cfg.tickRate);
    }

    animator.play("idle");
    frameWidth = animator.getFrameWidth();
    frameHeight = animator.getFrameHeight();
    setOffset(-frameWidth/2.0, -frameHeight);
    setPos(50,450);
}

void Player::update(qreal dt)
{
    if(currentState == EntityState::Dead)
    {
        applyGravity(dt, false, 0);
        return;
    }
    resetAttack(dt);
    updateInputDebounce();
    handleMove(dt);
    applyGravity(dt,jumpTrigger,myConfig.jumpSpeed);

}

void Player::resetAttack(qreal dt)
{
    if(currentState != EntityState::Attack && comboWindowTimer > 0.0f)
    {
        comboWindowTimer -= dt;
    }
    if(comboWindowTimer < 0.0f)
    {
        comboWindowTimer = 0.0f;
        isAttackBuffered = false;
        m_comboIndex = 1;
    }
}

void Player::updateInputDebounce()
{
    if(isJumpPressed && !wasJumpPressed)
    {
        jumpTrigger = true;
    }else
    {
        jumpTrigger = false;
    }
    wasJumpPressed = isJumpPressed;
}

void Player::handleInput(bool left, bool right, bool jump, bool attack)
{
    isLeftPressed = left;
    isRightPressed = right;
    isJumpPressed = jump;
    isAttackPressed = attack;
    if(isAttackPressed && !lastAttackPressed)
    {
        isAttackBuffered = true;
    }
    lastAttackPressed = isAttackPressed;
}

// Player movement logic
void Player::handleMove(qreal dt)
{
    if(currentState == EntityState::Attack)
    {
        advanceAttack(dt);
        return;
    }
    qreal moveDistance = myConfig.moveSpeed * dt;

    if(isRightPressed)
    {
        setX(x()+ moveDistance);
        setTransform(QTransform());
    }
    if(isLeftPressed)
    {
        setX(x()- moveDistance);
        setTransform(QTransform::fromScale(-1,1));
    }
    clampRolePos();
}

void Player::advanceAttack(qreal dt)
{
    if (isOnGround) {
        return;
    }
    else
    {
        qreal moveDistance = myConfig.moveSpeed * dt;
        bool facingRight = (transform().m11() > 0);
        if (facingRight) {
            if (isRightPressed) {
                setX(x() + moveDistance);
            }
        } else {
            if (isLeftPressed) {
                setX(x() - moveDistance);
            }
        }
        clampRolePos();
        return;
    }
}

void Player::clampRolePos()
{
    qreal halfWidth = 60;
    if(x() - halfWidth < minXBound)
    {
        setX(minXBound + halfWidth);
    }
    if(x() + halfWidth > maxXBound)
    {
        setX(maxXBound - halfWidth);
    }
}

bool Player::isAttackActive() const
{
    return(currentState == EntityState::Attack && animator.getCurrentFrame() == 1);
}

QRectF Player::getHurtBox() const
{
    qreal bodyWidth = 60;
    qreal bodyHeight = 125;

    qreal left = x() - (bodyWidth/2);
    qreal top = y()- (frameHeight * 0.5);

    return QRectF(left, top, bodyWidth, bodyHeight);
}

QRectF Player::getAttackBox() const
{
    qreal atkWidth = animator.getFrameWidth()* 0.3;
    qreal atkHeight = animator.getFrameHeight()* 0.4;

    qreal offSetX = animator.getFrameWidth()* 0.1;
    qreal top = y()- (animator.getFrameHeight()*0.4);
    qreal left;
    if(transform().m11() > 0)
    {
        left = x() + offSetX;
    }
    else{
        left = x() - atkWidth - offSetX;
    }
    return QRectF(left, top, atkWidth, atkHeight);
}

void Player::onHurt(int damage, Entity* attacker)
{
    currentState = EntityState::Hurt;
    qDebug()<<"the"<<myConfig.name <<"was hitted by"
             <<attacker->getName()<<"damage is"<< damage <<"Remaining hp:"<< stats.runtime.hp;
}

void Player::onDeath(int damage, Entity* attacker)
{
    currentState = EntityState::Dead;
    qDebug()<<"the "<<myConfig.name<<"was"<< attacker->getName()<<"killed!"<< "the Fatal damage:"<<damage;
}

void Player::handleAttackEnd()
{
    if(isAttackBuffered && m_comboIndex < maxComboCount)
    {
        startAttack();
        return;
    }

    m_comboIndex = 1;
    isAttackBuffered = false;
    comboWindowTimer = COMBO_WINDOW_TIME;

    if(!isOnGround) {
        currentState = EntityState::Jump;
    } else if(isLeftPressed || isRightPressed) {
        currentState = EntityState::Run;
    } else {
        currentState = EntityState::Idle;
    }
}

void Player::startAttack()
{
    if(currentState != EntityState::Attack || m_comboIndex > maxComboCount)
    {
        m_comboIndex = 1;
    }else{
        m_comboIndex++;
    }
    currentState = EntityState::Attack;
    isAttackBuffered = false;
    comboWindowTimer = COMBO_WINDOW_TIME;
}

void Player::updateState()
{
    switch(currentState)
    {
    case EntityState::Dead:
        return;
        break;
    case EntityState::Hurt:
        if(hurtStunTimer <= 0)
        {
            currentState = isOnGround? currentState = EntityState::Idle: currentState = EntityState::Jump;
        }else{
            return;
        }
        break;
    case EntityState::Attack:
        if(m_isDead || isStunned())
        {
            currentState = EntityState::Idle;
            m_comboIndex = 1;
            return;
        }
        if(animator.isFinshed())
        {
            handleAttackEnd();
            return;
            // if(!isOnGround)
            // {
            //     currentState = EntityState::Jump;
            // }
            // else if(isLeftPressed || isRightPressed)
            // {
            //     currentState = EntityState::Run;
            // }
            // else{
            //     currentState = EntityState::Idle;
            // }
        }
        break;
    case EntityState::Idle:
        if(isAttackBuffered)
        {
            startAttack();
            return;
        }
        if(!isOnGround)
        {
            currentState = EntityState::Jump;
        }
        else if(isLeftPressed || isRightPressed)
        {
            currentState = EntityState::Run;
        }
        break;
    case EntityState::Run:
        if(isAttackBuffered)
        {
            startAttack();
            return;
        }
        if(!isOnGround)
        {
            currentState = EntityState::Jump;
        }
        else if(!isLeftPressed && !isRightPressed)
        {
            currentState = EntityState::Idle;
        }
        break;
    case EntityState::Jump:
        if(isAttackBuffered)
        {
            startAttack();
            return;
        }
        if(isOnGround)
        {
            if(isLeftPressed || isRightPressed)
            {
                currentState = EntityState::Run;
            }
            else{
                currentState = EntityState::Idle;
            }
        }
        break;
    }
}

void Player::heal(int amount)
{
    stats.runtime.hp += amount;
    if(stats.runtime.hp > stats.base.maxHp)
    {
        stats.runtime.hp = stats.base.maxHp;
    }
}

void Player::replayMagic(int magic)
{
    stats.runtime.mp += magic;
    if(stats.runtime.mp > stats.base.maxMp)
    {
        stats.runtime.mp = stats.base.maxMp;
    }
}

void Player::deleteMagic(int magic)
{
    stats.runtime.mp -= magic;
    if(stats.runtime.mp < 0)
    {
        stats.runtime.mp = 0;
    }
}

void Player::addExp(int exp)
{

    if(stats.runtime.level >= stats.base.maxLevel)
    {
        stats.runtime.exp = stats.base.maxExp;
        stats.runtime.level = stats.base.maxLevel;
        return;
    }
    stats.runtime.exp += exp;

    while(stats.runtime.exp >= stats.base.maxExp)
    {
        stats.runtime.exp -= stats.base.maxExp;
        ++stats.runtime.level;
        upgradeState();
    }

    if(stats.runtime.level >= stats.base.maxLevel)
    {
        stats.runtime.level = stats.base.maxLevel;
        stats.runtime.exp = stats.base.maxExp;
    }
}

void Player::upgradeState()
{
    stats.base.maxHp = stats.base.maxHp * myConfig.hpGrowth;
    stats.base.maxMp = stats.base.maxMp * myConfig.mpGrowth;
    stats.base.maxExp = stats.base.maxExp * myConfig.expGrowth;

    stats.runtime.hp = stats.base.maxHp;
    stats.runtime.mp = stats.base.maxMp;
}