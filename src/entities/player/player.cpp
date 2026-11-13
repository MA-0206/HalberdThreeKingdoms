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
    animator.addAnimation("idle",QPixmap(path + "lb_idle"),18,2,9,6);
    animator.addAnimation("run",QPixmap(path + "lb_run"),7,1,7,3);
    animator.addAnimation("jump",QPixmap(path + "lb_jump"),23,3,8,4);
    animator.addAnimation("attack",QPixmap(path + "lb_attack"),11,2,6,2);
    animator.addAnimation("hurt",QPixmap(path + "lb_hurt"),9,2,5,3);
    animator.addAnimation("dead", QPixmap(path + "lb_dead"),16,2,8,4);

    animator.play("idle");
    setScale(1.0);
    frameWidth = animator.getFrameWidth();
    frameHeight = animator.getFrameHeight();
    setOffset(-frameWidth/2.0, -frameHeight);
    setPos(500,650);
}

void Player::update(qreal dt)
{
    if(currentState == EntityState::Dead)
    {
        applyGravity(dt, false, 0);
        return;
    }
    updateInputDebounce();
    handleMove(dt);
    applyGravity(dt,jumpTrigger,myConfig.jumpSpeed);

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
}

// Player movement logic
void Player::handleMove(qreal dt)
{
    qreal moveSpeed = 300.0;
    qreal moveDistance = moveSpeed * dt;

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
}

bool Player::isAttackActive() const
{
    return(currentState == EntityState::Attack && animator.getCurrentFrame() == 1);
}

QRectF Player::getAttackBox() const
{
    qreal atkWidth = animator.getFrameWidth()* 0.4;
    qreal atkHeight = animator.getFrameHeight()* 0.6;

    qreal offSetX = animator.getFrameWidth()* 0.1;
    qreal top = y()- (animator.getFrameHeight()*0.8);
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
        if(animator.isFinshed())
        {
            if(!isOnGround)
            {
                currentState = EntityState::Jump;
            }
            else if(isLeftPressed || isRightPressed)
            {
                currentState = EntityState::Run;
            }
            else{
                currentState = EntityState::Idle;
            }
        }
        break;
    case EntityState::Idle:
        if(isAttackPressed && currentState != EntityState::Attack)
        {
            currentState = EntityState::Attack;
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
        if(isAttackPressed && currentState != EntityState::Attack)
        {
            currentState = EntityState::Attack;
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
        if(isAttackPressed && currentState != EntityState::Attack)
        {
            currentState = EntityState::Attack;
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