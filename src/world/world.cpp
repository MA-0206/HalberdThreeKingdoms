#include "world.h"

World::World()
{
    CharacterConfig liubeiConfig;
    player = new Player(liubeiConfig);
}

World::~World()
{
    delete player;
    for(Enemy* e: enemies){
        delete e;
    }
    enemies.clear();
}

void World::update(qreal dt, bool left, bool right, bool jump, bool attack)
{
    player->handleInput(left, right, jump, attack);
    player->update(dt);

    for(Enemy* enemy:enemies)
    {
        enemy->update(dt);
    }
}

// Generate a mini-monster
Enemy* World::spawnTestEnemy(qreal x, qreal y)
{
    EnemyConfig enemyConfig;
    Enemy* newEnemy = new Enemy(enemyConfig);
    newEnemy->setPos(x,y);
    enemies.push_back(newEnemy);
    return newEnemy;
}