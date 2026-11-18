#ifndef JSONPARSER_H
#define JSONPARSER_H

#include<QString>
#include<map>
#include"player.h"
#include "enemy.h"

class JsonParser
{
public:
    static JsonParser& getInstance(){
        static JsonParser instance;
        return instance;}

    void loadAllConfigs();
    CharacterConfig getCharacterConfig(const QString& charId);
    EnemyConfig getEnemyConfig(const QString& charId);
private:
    JsonParser() = default;


    const std::string CHARACTERS_PATH = "configs/characters.json";
    const std::string ENEMIES_PATH = "configs/enemy.json";
    const std::string LEVEL_PATH = "configs/level";

    void parserCharacterConfig();
    void parserEnemyConfig();
    void parserLevelConfig();

    std::map<std::string,CharacterConfig> m_characters;
    std::map<std::string,EnemyConfig> m_enemies;
};

#endif // JSONPARSER_H
