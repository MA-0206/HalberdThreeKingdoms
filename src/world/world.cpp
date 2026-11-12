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
    player->tick(dt);

    for(Enemy* enemy:enemies)
    {
        if(enemy->getTarget()== nullptr || enemy->getTarget()->isDead())
        {
            enemy->setTarget(player);
        }
        enemy->tick(dt);
    }
    checkCombat();
    cleanUpDeadEnemies();
}

void World::checkCombat()
{
    if(player->isAttackActive())
    {
        QRectF ackBox = player->getAttackBox();
        for(Enemy* enemy: enemies)
        {
            if(canAttack(player->faction, enemy->faction) && ackBox.intersects(enemy->getHurtBox()))
            {
                enemy->takeDamage(20, player);
            }
        }
    }
    for(Enemy* enemy: enemies)
    {
        if(enemy->isAttackActive())
        {
            QRectF ackBox = enemy->getAttackBox();
            if(canAttack(enemy->faction, player->faction) && ackBox.intersects(player->getHurtBox()))
            {
                player->takeDamage(10, enemy);
            }
        }
    }
}

void World::cleanUpDeadEnemies()
{
    for(auto it = enemies.begin();it!= enemies.end();)
    {
        Enemy* e = *it;
        if(e->canBeRemoved())
        {
            if(e->killer != nullptr)
            {
                Player* p = dynamic_cast<Player*>(e->killer);
                if(p)
                {
                    p->addExp(e->getExpReward());
                    qDebug()<< "World:"<< p->getName()<< "killed"<< e->getName()
                             << "acqure exp:"<< e->getExpReward();
                }
            }
            delete e;
            it = enemies.erase(it);
        }else{
            ++it;
        }
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