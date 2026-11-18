#ifndef ENEMY_H
#define ENEMY_H

#include <entity.h>

struct EnemyConfig{
    QString name;
    int maxHp;
    int expReward;
    qreal sightRange;
    qreal attackRange;
    qreal attackCD;
    qreal lostTargetWaitTime;
    qreal runSpeed;
    qreal chaseSpeed;
    qreal patrolRange;
    qreal patrolMinX;
    qreal patrolMaxX;
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
