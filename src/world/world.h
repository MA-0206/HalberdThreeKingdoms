#ifndef WORLD_H
#define WORLD_H

#include<vector>
#include"player.h"
#include"enemy.h"

class World
{
public:
    World();
    ~World();

    void update(qreal dt, bool left, bool right, bool jump, bool attack);
    void checkCombat();
    void cleanUpDeadEnemies();
    Player* getPlayer()const {return player;}
    Enemy* spawnTestEnemy(qreal x, qreal y);

private:
    Player* player;
    std::vector<Enemy*> enemies;
};

#endif // WORLD_H
