#ifndef ENTITYDATA_H
#define ENTITYDATA_H

#include <QString>

enum class EntityState{
    Idle,
    Run,
    Jump,
    Attack,
    Chase,
    Hurt,
    Dead
};

enum class Faction{
    Player,
    Enemy,
    Neutral
};

struct BaseAttributes {
    QString name;
    int maxHp = 1;
    int maxMp = 1;
    int maxExp = 1;
    int maxLevel = 1;
};

struct RunTimeState {
    QString name;
    int hp = 1;
    int mp = 1;
    int exp = 1;
    int level = 1;
};

struct EntityStats {
    BaseAttributes base;
    RunTimeState runtime;
};

inline bool canAttack(Faction attacker, Faction victim)
{
    if(attacker == victim) return false;
    if(attacker == Faction::Player) return true;
    if(attacker == Faction::Enemy) return (victim == Faction::Player);
    if(attacker == Faction::Neutral) return false;
    return false;
}

#endif // ENTITYDATA_H
