#include "world.h"
#include "jsonparser.h"

World::World(const QString& characterId, const QString& levelId)
{
    initPlayer(characterId);
    initLevel(levelId);
}

World::~World()
{
    delete player;
    for(Enemy* e: enemies){
        delete e;
    }
    enemies.clear();
}

void World::initPlayer(const QString& characterId)
{
    CharacterConfig playerConfig = JsonParser::getInstance().getCharacterConfig(characterId);
    player = new Player(playerConfig);
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

void World::initLevel(const QString& levelId)
{
    qDebug()<<"the initbattleZone is start";
    m_maxReachedX = player->x();
    JsonParser::getInstance().loadLevel(levelId, m_levelMapPath, m_mapTotalWidth, m_mapTotalHeight, m_battleZones);
}

void World::updateBattleZone(qreal dt)
{
    checkZoneTrigger();
    if(!m_battleZones[m_currentZoneIndex].isTriggered || m_battleZones[m_currentZoneIndex].isCleared) return;
    if(m_battleZones[m_currentZoneIndex].waveIntervalTimer > 0.0)
    {
        m_battleZones[m_currentZoneIndex].waveIntervalTimer -= dt;
        return;
    }
    updateWaveSpawning(dt);
    checkWaveCleared();
}

void World::checkZoneTrigger()
{
    int nextZoneIndex = m_currentZoneIndex +1;
    if(nextZoneIndex >= m_battleZones.size()) return;
    if(player->getHurtBox().intersects(m_battleZones[nextZoneIndex].triggerBox))
    {
        m_currentZoneIndex = nextZoneIndex;
        m_battleZones[nextZoneIndex].isTriggered = true;

        player->minXBound = m_battleZones[nextZoneIndex].lockLeftX;
        player->maxXBound = m_battleZones[nextZoneIndex].lockRightX;

        m_cameraMinX = m_battleZones[nextZoneIndex].lockLeftX;;
        m_cameraMaxX = m_battleZones[nextZoneIndex].lockRightX;

        for(Enemy* e:enemies)
        {
            e->minXBound = m_battleZones[nextZoneIndex].lockLeftX;;
            e->maxXBound = m_battleZones[nextZoneIndex].lockRightX;
        }
        qDebug()<<"Start fight!";
    }
}

void World::updateWaveSpawning(qreal dt)
{
    if(m_currentZoneIndex < 0 || m_currentZoneIndex >= m_battleZones.size())
    {
        qDebug()<<"updateWaveSpawning:the m_currentZoneIndex is invalied";
        return;
    }
    BattleZone& m_zone = m_battleZones[m_currentZoneIndex];
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
    if(m_currentZoneIndex < 0 || m_currentZoneIndex >= m_battleZones.size())
    {
        qDebug() << "checkWaveCleared:the m_currentZoneIndex is invalied";
        return;
    }
    BattleZone& m_zone = m_battleZones[m_currentZoneIndex];
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
    if(m_currentZoneIndex < 0 || m_battleZones[m_currentZoneIndex].isCleared)
    {
        if(player->x() > m_maxReachedX)
        {
            m_maxReachedX = player->x();
        }
        qreal newLeftBound = m_maxReachedX - 640.0;
        player->minXBound = std::max(newLeftBound,0.0);
        m_cameraMinX = std::max(newLeftBound,0.0);
    }
}

// Generate a mini-monster
Enemy* World::spawnTestEnemy(qreal x, qreal y)
{
    EnemyConfig enemyConfig = JsonParser::getInstance().getEnemyConfig("yellowscarf");
    Enemy* newEnemy = new Enemy(enemyConfig);
    newEnemy->setPos(x,y - 200);
    newEnemy->minXBound = m_battleZones[m_currentZoneIndex].lockLeftX;
    newEnemy->maxXBound = m_battleZones[m_currentZoneIndex].lockRightX;
    enemies.push_back(newEnemy);
    pendingEnemies.push_back(newEnemy);
    return newEnemy;
}