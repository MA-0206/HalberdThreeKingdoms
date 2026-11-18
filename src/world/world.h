#ifndef WORLD_H
#define WORLD_H

#include<vector>
#include"player.h"
#include"enemy.h"

struct SpawnPoint {
    qreal x;
    qreal y;
    qreal delay;
    bool hasSpawned = false;
};

struct WaveConfig {
    std::vector<SpawnPoint> spawnPoints;
};

struct BattleZone {
    QRectF triggerBox;
    qreal lockLeftX;
    qreal lockRightX;

    std::vector<WaveConfig> waves;

    int currentWaveIndex = 0;
    qreal waveTimer = 0.0;
    bool isTriggered = false;
    bool isCleared = false;
    qreal waveIntervalTimer = 0.0;
};

class World
{
public:
    World();
    ~World();

    void update(qreal dt, bool left, bool right, bool jump, bool attack);
    void checkCombat();
    void cleanUpDeadEnemies();
    void clearPendingEnemies(){pendingEnemies.clear();}
    std::vector<Enemy*>& getPendingEnemies(){return pendingEnemies;}
    qreal getCameraMinX() const {return m_cameraMinX;}
    qreal getCameraMaxX() const {return m_cameraMaxX;}
    Player* getPlayer()const {return player;}
    Enemy* spawnTestEnemy(qreal x, qreal y);

private:
    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> pendingEnemies;

    BattleZone m_zone;
    qreal m_mapTotalWidth = 5000.0;

    qreal m_cameraMinX = 0.0;;
    qreal m_cameraMaxX = 5000.0;
    qreal m_maxReachedX = 0.0;

    void initPlayer();
    void initBattleZone();
    void updateBattleZone(qreal dt);
    void checkZoneTrigger();
    void updateWaveSpawning(qreal dt);
    void checkWaveCleared();
    void preventRollback();
};

#endif // WORLD_H
