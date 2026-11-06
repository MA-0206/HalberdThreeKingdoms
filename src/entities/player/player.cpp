#include "player.h"

Player::Player(CharacterConfig& config, QGraphicsItem* parent):Entity(parent),myConfig(config)
{
    initStats();
    loadPlayerAssets();
}

void Player::initStats()
{
    stats.runtime.level = 1;
    stats.base.maxHp = myConfig.baseHp;
    stats.base.maxMp = myConfig.baseMp;
    stats.base.maxExp = myConfig.baseExp;

    stats.runtime.hp = stats.base.maxHp;
    stats.runtime.mp = stats.base.maxMp;
    stats.runtime.exp = 0;

}

// Load the sprite sheet and calculate the size
void Player::loadPlayerAssets()
{
    animator.addAnimation("idle",QPixmap(":/assets/images/characters/liubei/lb_idle"),18,2,9,6);
    animator.addAnimation("run",QPixmap(":/assets/images/characters/liubei/lb_run"),7,1,7,3);
    animator.addAnimation("jump",QPixmap(":/assets/images/characters/liubei/lb_jump"),23,3,8,4);
    animator.addAnimation("attack",QPixmap(":/assets/images/characters/liubei/lb_attack"),11,2,6,2);

    animator.play("idle");
    setScale(1.0);
    frameWidth = animator.getFrameWidth();
    frameHeight = animator.getFrameHeight();
    setOffset(-frameWidth/2.0, -frameHeight);
    setPos(500,650);
}

void Player::update(qreal dt)
{
    handleMove(dt);
    applyGravity(dt,isJumpPressed,-800);
    updateState();
    updateAnimation();
}

void Player::handleInput(bool left, bool right, bool jump, bool attack)
{
    isLeftPressed = left;
    isRightPressed = right;
    isJumpPressed = jump;
    isAttackPressed = attack;
}

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

void Player::takeDamage(int damage)
{
    stats.runtime.hp -= damage;
    if(stats.runtime.hp < 0)
    {
        stats.runtime.hp = 0;
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

void Player::updateState()
{
    if(isAttackPressed && currentState != EntityState::Attack)
    {
        currentState = EntityState::Attack;
        return;
    }
    switch(currentState)
    {
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