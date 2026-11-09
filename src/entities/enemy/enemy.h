#ifndef ENEMY_H
#define ENEMY_H

#include <entity.h>

struct EnemyConfig{
    QString spriteFolder = "yellowscarf";
    int maxHp = 100;
    int runSpeed = 100;
    qreal patrolMinX = 500.0;
    qreal patrolMaxX = 800.0;
};

class Enemy : public Entity
{
public:
    Enemy(const EnemyConfig& config, QGraphicsItem* parent = nullptr);
    ~Enemy() = default;

    void update(qreal dt)override;

protected:
    void updateState()override;

private:
    EnemyConfig myConfig;
    int moveDirection = 1;

    bool isWaitingAtEdge = false;
    qreal waitTimer = 0.0;
    const qreal waitDuration = 1.5;

    void updatePatrol(qreal dt);
    void loadEnemyAssets();
};

#endif // ENEMY_H
