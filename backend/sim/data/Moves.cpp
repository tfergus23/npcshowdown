#include "sim/data/Moves.hpp"
#include "sim/utils/MoveFunctions.hpp"
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

const Move MOVE_NONE = {
    .name = "",
    .type = Type::NONE,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 0,
    .maxPP = 0,
    .priority = 8,
    .critRatio = 0,
    .targetType = TargetType::SELF,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 0
};

const Move MOVE_SWITCH = {
    .name = "Switch",
    .type = Type::NONE,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 0,
    .maxPP = 0,
    .priority = 8,
    .critRatio = 0,
    .targetType = TargetType::SELF,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 1
};

const Move MOVE_STRUGGLE = {
    .name = "Struggle",
    .type = Type::NONE,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 50,
    .accuracy = 0,
    .maxPP = 1,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 2,

    //Flags
    .contact = true,
    .protect = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamageWithRecoil(myMove, 1.0f / 4.0f, true);
    }
};

const Move MOVE_POUND = {
    .name = "Pound",
    .type = Type::NORMAL,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 40,
    .accuracy = 100,
    .maxPP = 56,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 3,

    //Flags
    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

const Move MOVE_TACKLE = {
    .name = "Tackle",
    .type = Type::NORMAL,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 40,
    .accuracy = 100,
    .maxPP = 56,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 4,

    //Flags
    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

const Move MOVE_KARATE_CHOP = {
    .name = "Karate Chop",
    .type = Type::FIGHTING,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 50,
    .accuracy = 100,
    .maxPP = 40,
    .priority = 0,
    .critRatio = 1,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 5,

    //Flags
    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

const Move MOVE_SURF = {
    .name = "Surf",
    .type = Type::WATER,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 6,

    //Flags
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

const Move MOVE_FLAMETHROWER = {
    .name = "Flamethrower",
    .type = Type::FIRE,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::BURN,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 53,

    //Flags
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        int damage = dealDirectDamage(myMove);
        if (damage > 0) applySecondaryEffect(myMove, opponentMove);
    }
};

const Move MOVE_CONFUSION_HIT = {
    .name = "",
    .type = Type::NONE,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 40,
    .accuracy = 0,
    .maxPP = 0,
    .priority = 8,
    .critRatio = 0,
    .targetType = TargetType::SELF,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 0,
    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        dealDirectDamage(myMove);
    }
};

const std::unordered_map<std::string, const Move*> moves = {
    {MOVE_NONE.name, &MOVE_NONE},
    {MOVE_POUND.name, &MOVE_POUND},
    {MOVE_TACKLE.name, &MOVE_TACKLE},
    {MOVE_KARATE_CHOP.name, &MOVE_KARATE_CHOP},
    {MOVE_SURF.name, &MOVE_SURF},
    {MOVE_FLAMETHROWER.name, &MOVE_FLAMETHROWER}
};

static std::unordered_map<int16_t, const Move*> idToMoveMap;

void mapIDToMove(int16_t id, const Move* move){
    assert(!idToMoveMap.contains(id));
    assert(id == move->id);
    idToMoveMap[id] = move;
}

void mapIDsToMoves(){
    assert(moves.size() > 0);
    for (const auto& [str, move] : moves){
        mapIDToMove(move->id, move);
    }
}
const Move* moveFromID(int16_t id){
    return idToMoveMap.at(id);
}

const Move* moveFromString(const std::string& moveName) {
    return moves.at(moveName);
}

std::string createMoveDataResponse(){
    std::vector<std::string> moveNames;
    for (auto [name,ptr] : moves){
        moveNames.push_back(name);
    }
    std::sort(moveNames.begin(), moveNames.end());
    json response;
    response["success"] = true;
    response["message"] = "OK";
    response["data"] = moveNames;
    return response.dump();
}