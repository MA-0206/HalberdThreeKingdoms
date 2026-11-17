#include "world.h"


World::World()
{
    CharacterConfig liubeiConfig;
    player = new Player(liubeiConfig);
    initBattleZone();
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
    updateBattleZone(dt);
    cleanUpDeadEnemies();
    preventRollback();
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

void World::initBattleZone()
{
    m_maxReachedX = player->x();
    m_zone.triggerBox = QRectF(1200, 0, 50, 720);
    m_zone.lockLeftX = 500;
    m_zone.lockRightX = 2200;

    WaveConfig wave1;
    wave1.spawnPoints.push_back({1100.0,650.0 ,0.0, false});
    wave1.spawnPoints.push_back({2000.0, 650.0, 0.5, false});
    m_zone.waves.push_back(wave1);

    WaveConfig wave2;
    wave2.spawnPoints.push_back({1100.0, 650.0, 0.0, false});
    wave2.spawnPoints.push_back({1150.0,650.0, 0.5,false});
    wave2.spawnPoints.push_back({2000.0, 650.0, 0.0, false});
    wave2.spawnPoints.push_back({2050.0, 650.0,0.0, false});
    m_zone.waves.push_back(wave2);
}

void World::updateBattleZone(qreal dt)
{
    checkZoneTrigger();
    if(!m_zone.isTriggered || m_zone.isCleared) return;
    if(m_zone.waveIntervalTimer > 0.0)
    {
        m_zone.waveIntervalTimer -= dt;
        return;
    }
    updateWaveSpawning(dt);
    checkWaveCleared();
}

void World::checkZoneTrigger()
{
    if(m_zone.isTriggered) return;
    if(player->getHurtBox().intersects(m_zone.triggerBox))
    {
        m_zone.isTriggered = true;

        player->minXBound = m_zone.lockLeftX;
        player->maxXBound = m_zone.lockRightX;

        m_cameraMinX = m_zone.lockLeftX;
        m_cameraMaxX = m_zone.lockRightX;

        for(Enemy* e:enemies)
        {
            e->minXBound = m_zone.lockLeftX;
            e->maxXBound = m_zone.lockRightX;
        }
        qDebug()<<"Start fight!";
    }
}

void World::updateWaveSpawning(qreal dt)
{
    if(m_zone.currentWaveIndex >= m_zone.waves.size()) return;
    m_zone.waveTimer += dt;

    WaveConfig& currentWave = m_zone.waves[m_zone.currentWaveIndex];
    for(auto& sp : currentWave.spawnPoints)
    {
        if(!sp.hasSpawned && m_zone.waveTimer >= sp.delay)
        {
        spawnTestEnemy(sp.x,sp.y);
            sp.hasSpawned = true;
        }
    }
}

void World::checkWaveCleared()
{
    if(m_zone.currentWaveIndex >= m_zone.waves.size()) return;
    WaveConfig& currentWave = m_zone.waves[m_zone.currentWaveIndex];
    bool allSpawned = true;
    for(auto& sp:currentWave.spawnPoints)
    {
        if(!sp.hasSpawned)
        {
            allSpawned = false;
            break;
        }
    }

    bool allDead = true;
    for(Enemy* e:enemies)
    {
        if(!e->isDead())
        {
            allDead = false;
            break;
        }
    }
    if(allDead && m_zone.waveTimer >= 1.0)
    {
        m_zone.currentWaveIndex++;
        m_zone.waveTimer = 0.0;

        if(m_zone.currentWaveIndex >= m_zone.waves.size())
        {
            m_zone.isCleared = true;
            player->maxXBound = m_mapTotalWidth;
            m_cameraMaxX = m_mapTotalWidth;
            qDebug()<<"all enemy dead";
        }else{
            m_zone.waveIntervalTimer = 1.5;
            qDebug()<<"第"<< m_zone.currentWaveIndex+1 << "波!";
        }
    }
}

void World::preventRollback()
{
    if(!m_zone.isTriggered || m_zone.isCleared)
    {
        if(player->x() > m_maxReachedX)
        {
            m_maxReachedX = player->x();
        }
        qreal newLeftBound = m_maxReachedX - 700.0;
        player->minXBound = std::max(newLeftBound,0.0);
        m_cameraMinX = std::max(newLeftBound,0.0);
    }
}

// Generate a mini-monster
Enemy* World::spawnTestEnemy(qreal x, qreal y)
{
    EnemyConfig enemyConfig;
    Enemy* newEnemy = new Enemy(enemyConfig);
    newEnemy->setPos(x,y - 200);
    newEnemy->minXBound = m_zone.lockLeftX;
    newEnemy->maxXBound = m_zone.lockRightX;
    enemies.push_back(newEnemy);
    pendingEnemies.push_back(newEnemy);
    return newEnemy;
}