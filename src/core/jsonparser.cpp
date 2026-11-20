#include "jsonparser.h"
#include "json.hpp"
#include <fstream>
#include <QDebug>

void JsonParser::loadLevel(const QString& levelName, QString& outLevelMap, qreal& outMapWidth,
                           qreal& outMapHeight, std::vector<BattleZone>& outZones)
{
    std::ifstream file(LEVEL_PATH + levelName.toStdString() + ".json");
    if(!file.is_open())
    {
        qDebug()<< "the level:" << levelName << "load path open failed";
        return;
    }
    nlohmann::json j;
    file >> j;

    outLevelMap  = QString::fromStdString(j.value<std::string>("levelMapPath",""));
    outMapWidth  = j.value<qreal>("mapWidth",4000.0);
    outMapHeight = j.value<qreal>("mapHeight",400.0);

    outZones.clear();
    if(j.contains("zones") && j["zones"].is_array())
    {
        for(const auto& zonejson : j["zones"])
        {
            BattleZone zone;
            zone.lockLeftX = zonejson.value<qreal>("lockLeftX",0.0);
            zone.lockRightX = zonejson.value<qreal>("lockRightX",outMapWidth);
            qreal triggerX = zonejson.value<qreal>("triggerX",0.0);
            zone.triggerBox = QRectF(triggerX, 0, 50, outMapHeight);

            zone.isTriggered = false;
            zone.isCleared = false;
            zone.currentWaveIndex = 0;
            zone.waveTimer = 0.0;
            zone.waveIntervalTimer = 0.0;

            if(zonejson.contains("waves") && zonejson["waves"].is_array())
            {
                for(const auto& wavejson : zonejson["waves"])
                {
                    WaveConfig wave;
                    for(const auto& spawnjson : wavejson)
                    {
                        SpawnPoint sp;
                        sp.x = spawnjson.value<qreal>("x", 0.0);
                        sp.y = spawnjson.value<qreal>("y", 0.0);
                        sp.delay = spawnjson.value<qreal>("delay",0.0);
                        sp.hasSpawned = false;

                        wave.spawnPoints.push_back(sp);
                    }
                    zone.waves.push_back(wave);
                }
            }
            outZones.push_back(zone);
        }
    }
    qDebug()<<"level:" << levelName << "is load successed! the fight area has:"<< outZones.size() ;
}


void JsonParser::loadAllConfigs()
{
    parserCharacterConfig();
    parserEnemyConfig();
}

void JsonParser::parserCharacterConfig()
{
    std::ifstream file(CHARACTERS_PATH);
    if(!file.is_open())
    {
        qDebug()<<"the characters json path is not open";
        return;
    }
    nlohmann::json j;
    file >> j;

    for(auto& [key,value]: j.items())
    {
        CharacterConfig config;
        config.name = QString::fromStdString(value.value<std::string>("name","unkown"));
        config.avatarPath = QString::fromStdString(value.value<std::string>("avatarPath","unkownPath"));
        config.baseHp = value.value<int>("baseHp",0);
        config.baseMp = value.value<int>("baseMp",0);
        config.baseExp = value.value<int>("baseExp",0);
        config.baseAttack = value.value<int>("baseAttack",0);
        config.hpGrowth = value.value<qreal>("hpGrowth",0.0);
        config.attackGrowth = value.value<qreal>("attackGrowth",0.0);
        config.hpRegenPerSec = value.value<qreal>("hpRegenPerSec",0.0);
        config.mpGrowth = value.value<qreal>("mpGrowth",0.0);
        config.expGrowth = value.value<qreal>("expGrowth",0.0);
        config.moveSpeed = value.value<qreal>("moveSpeed",0.0);
        config.jumpSpeed = value.value<qreal>("jumpSpeed",0.0);
        if(value.contains("animations"))
        {
            for(auto& [animName,animData]:value["animations"].items())
            {
                AnimationConfig anim;
                anim.totalFrames = animData.value<int>("totalFrames",1);
                anim.rows = animData.value<int>("rows",1);
                anim.cols = animData.value<int>("cols",1);
                anim.tickRate = animData.value<int>("tickRate",1);

                config.animations[QString::fromStdString(animName)] = anim;
            }
        }
        m_characters[key] = std::move(config);
        qDebug()<< "parserCharacterConfig is successed:" << QString::fromStdString(key);
    }
}

void JsonParser::parserEnemyConfig()
{
    std::ifstream file(ENEMIES_PATH);
    if(!file.is_open())
    {
        qDebug()<< "the enemy json path is not open";
        return;
    }

    nlohmann::json j;
    file >> j;
    for(auto& [key,value]:j.items())
    {
        EnemyConfig config;
        config.name = QString::fromStdString(value.value<std::string>("name","unkown"));
        config.maxHp = value.value<int>("maxHp",0.0);
        config.expReward = value.value<int>("expReward",0.0);
        config.sightRange = value.value<qreal>("sightRange",0.0);
        config.attackRange = value.value<qreal>("attackRange",0.0);
        config.attackCD = value.value<qreal>("attackCD",0.0);
        config.lostTargetWaitTime = value.value<qreal>("lostTargetWaitTime",0.0);
        config.runSpeed = value.value<qreal>("runSpeed",0.0);
        config.chaseSpeed = value.value<qreal>("chaseSpeed",0.0);
        config.patrolRange = value.value<qreal>("patrolRange",0.0);
        config.patrolMinX = value.value<qreal>("patrolMinX",0.0);
        config.patrolMaxX = value.value<qreal>("patrolMaxX",0.0);

        m_enemies[key] = std::move(config);
        qDebug()<<"parseEnemyConfig is successed:" << QString::fromStdString(key);
    }
}

CharacterConfig JsonParser::getCharacterConfig(const QString& charId)
{
    std::string id = charId.toStdString();
    if(m_characters.find(id) != m_characters.end())
    {
        qDebug()<< "the characterConfig is successed get:" << charId;
        return m_characters[id];
    }
    qDebug()<< "the characterConfig is not get:" << charId;
    return CharacterConfig();
}

EnemyConfig JsonParser::getEnemyConfig(const QString& charId)
{
    std::string id = charId.toStdString();
    if(m_enemies.find(id) != m_enemies.end())
    {
        qDebug()<< "the characterConfig is successed get:" << charId;
        return m_enemies[id];
    }
    qDebug()<< "the enemyConfig is not get:" << charId;
    return EnemyConfig();
}