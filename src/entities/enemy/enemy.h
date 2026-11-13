#ifndef ENEMY_H
#define ENEMY_H

#include <entity.h>

struct EnemyConfig{
    QString name = "yellowscarf";
    int maxHp = 200;
    int expReward = 650;
    qreal sightRange = 500.0;
    qreal attackRange = 80.0;
    qreal attackCD   = 3.5;
    qreal lostTargetWaitTime = 2.0;
    qreal runSpeed = 100.0;
    qreal chaseSpeed = 150.0;
    qreal patrolRange = 400.0;
    qreal patrolMinX = 500.0;
    qreal patrolMaxX = 800.0;
};

class Enemy : public Entity
{
public:
    Enemy(const EnemyConfig& config, QGraphicsItem* parent = nullptr);
    ~Enemy() = default;

    void update(qreal dt) override;
    int getExpReward() const { return myConfig.expReward; }
    bool isAttackActive() const override;
    QRectF getAttackBox() const override;
    Entity* getTarget() const {return m_target;}
    void setTarget(Entity* target);

protected:
    void updateState() override;
    void onHurt(int damage, Entity* attacker) override;
    void onDeath(int damage, Entity* killer) override;

private:
    EnemyConfig myConfig;
    int moveDirection = 1;

    Entity* m_target = nullptr;
    bool isWaitingAtEdge = false;
    qreal waitTimer = 0.0;
    qreal lostTimer = 0.0;
    qreal hitCoolTimer = 0.0;
    qreal patrolCenterX = 0.0;
    const qreal waitDuration = 1.5;

    void loadEnemyAssets();
    void initStats();
    void updatePatrol(qreal dt);
    void updateChase(qreal dt);
    void handleAction(qreal dt);
    void handleEdgeWaitTimeout();
    void handleLostWaitTimeout();
    bool hasValidTarget() const;
    bool isTargetInAttackRange() const;
    bool isTargetInSightRange() const;
};

#endif // ENEMY_H
