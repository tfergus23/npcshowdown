#pragma once
#include "sim/battle/Move.hpp"
#include "sim/utils/move_functions.hpp"

inline const Move* MOVE_NONE = nullptr;
inline const std::string MOVE_NONE_NAME = "";

//TODO: implement these
class Switch : public Move {
public:
    Switch() {}
};
class Struggle : public Move {
public:
    Struggle() {}
};

inline const Switch MOVE_SWITCH;
inline const Struggle MOVE_STRUGGLE;

inline const Move MOVE_POUND = {
    .name = "Pound",
    .type = Type::NORMAL,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 40,
    .accuracy = 100,
    .maxPP = 56,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NOEFFECT,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,

    //Flags
    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

inline const Move MOVE_TACKLE = {
    .name = "Tackle",
    .type = NORMAL,
    .damageCategory = PHYSICAL,
    .power = 40,
    .accuracy = 100,
    .maxPP = 56,
    .priority = 0,
    .critRatio = 0,
    .targetType = OPPONENT,
    .secondaryEffect = NOEFFECT,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,

    //Flags
    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

inline const Move MOVE_KARATE_CHOP = {
    .name = "Karate Chop",
    .type = FIGHTING,
    .damageCategory = PHYSICAL,
    .power = 50,
    .accuracy = 100,
    .maxPP = 40,
    .priority = 0,
    .critRatio = 1,
    .targetType = OPPONENT,
    .secondaryEffect = NOEFFECT,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,

    //Flags
    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

inline const std::unordered_map<std::string, const Move*> moves = {
    {MOVE_NONE_NAME, MOVE_NONE},
    {MOVE_POUND.name, &MOVE_POUND},
    {MOVE_TACKLE.name, &MOVE_TACKLE},
    {MOVE_KARATE_CHOP.name, &MOVE_KARATE_CHOP}
};

inline const Move* getMove(const std::string& moveName) {
    return moves.at(moveName);
}