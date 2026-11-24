#ifndef JSONPARSER_H
#define JSONPARSER_H

#include<QString>
#include<map>
#include"player.h"
#include "enemy.h"
#include "world.h"

struct PortraitConfig {
    QString staticPath;
    QString dynamicPath;
    int totalFrames;
    int rows;
    int cols;
    int tickRate;
};

class JsonParser
{
public:
    static JsonParser& getInstance(){
        static JsonParser instance;
        return instance;}

    void loadAllConfigs();
    CharacterConfig getCharacterConfig(const QString& charId);
    EnemyConfig getEnemyConfig(const QString& charId);
    PortraitConfig getPortraitConfig(const QString& charId);
    void loadLevel(const QString& levelName, QString& outLevelMap, qreal& outMapWidth,
                   qreal& outMapHeight, std::vector<BattleZone>& outZones);
private:
    JsonParser() = default;


    const std::string CHARACTERS_PATH = "configs/characters.json";
    const std::string ENEMIES_PATH = "configs/enemy.json";
    const std::string LEVEL_PATH = "configs/levels/";
    const std::string PORTRAIT_PATH = "configs/ui/selectRole.json";

    void parserCharacterConfig();
    void parserEnemyConfig();
    void parserPortraitConfig();

    std::map<std::string,CharacterConfig> m_characters;
    std::map<std::string,EnemyConfig> m_enemies;
    std::map<std::string,PortraitConfig> m_portraits;
};

#endif // JSONPARSER_H
