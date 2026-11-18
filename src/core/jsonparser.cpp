#include "jsonparser.h"
#include "json.hpp"
#include <fstream>
#include <QDebug>

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
        config.name = QString::fromStdString(value["name"].get<std::string>());
        config.baseHp = value["baseHp"].get<int>();
        config.baseMp = value["baseMp"].get<int>();
        config.baseExp = value["baseExp"].get<int>();
        config.baseAttack = value["baseAttack"].get<int>();
        config.hpGrowth = value["hpGrowth"].get<qreal>();
        config.attackGrowth = value["attackGrowth"].get<qreal>();
        config.hpRegenPerSec = value["hpRegenPerSec"].get<qreal>();
        config.mpGrowth = value["mpGrowth"].get<qreal>();
        config.expGrowth = value["expGrowth"].get<qreal>();
        config.moveSpeed = value["moveSpeed"].get<qreal>();
        config.jumpSpeed = value["jumpSpeed"].get<qreal>();

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
        config.name = QString::fromStdString(value["name"].get<std::string>());
        config.maxHp = value["maxHp"].get<int>();
        config.expReward = value["expReward"].get<int>();
        config.sightRange = value["sightRange"].get<qreal>();
        config.attackRange = value["attackRange"].get<qreal>();
        config.attackCD = value["attackCD"].get<qreal>();
        config.lostTargetWaitTime = value["lostTargetWaitTime"].get<qreal>();
        config.runSpeed = value["runSpeed"].get<qreal>();
        config.chaseSpeed = value["chaseSpeed"].get<qreal>();
        config.patrolRange = value["patrolRange"].get<qreal>();
        config.patrolMinX = value["patrolMinX"].get<qreal>();
        config.patrolMaxX = value["patrolMaxX"].get<qreal>();

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