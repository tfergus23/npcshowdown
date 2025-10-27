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
        MoveFunctions::dealDirectDamageWithRecoil(myMove, opponentMove, 1.0f / 4.0f, true);
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
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
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
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
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
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
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
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
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
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
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
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_HYPER_VOICE = {
    .name = "Hyper Voice",
    .type = Type::NORMAL,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 304,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,
    .soundBased = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_RETURN = {
    .name = "Return",
    .type = Type::NORMAL,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 102,
    .accuracy = 100,
    .maxPP = 32,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 216,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_CLOSE_COMBAT = {
    .name = "Close Combat",
    .type = Type::FIGHTING,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 120,
    .accuracy = 100,
    .maxPP = 8,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 370,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
        MoveFunctions::changeStatModifier(Stat::DEFENSE, -1, myMove->user, myMove->battle, myMove);
        MoveFunctions::changeStatModifier(Stat::SPDEFENSE, -1, myMove->user, myMove->battle, myMove);
    }
};

const Move MOVE_AURA_SPHERE = {
    .name = "Aura Sphere",
    .type = Type::FIGHTING,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 0,
    .maxPP = 32,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 396,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_FLARE_BLITZ = {
    .name = "Flare Blitz",
    .type = Type::FIRE,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 120,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::BURN,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 394,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamageWithRecoil(myMove,opponentMove, 1.0f / 3.0f);
    }
};

const Move MOVE_WATERFALL = {
    .name = "Waterfall",
    .type = Type::WATER,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::FLINCH,
    .secondaryEffectChance = 20.0f,
    .secondaryEffectValue = -1,
    .id = 127,

    .contact = true,
    .protect = true,
    .mirrorMove = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_WILD_CHARGE = {
    .name = "Wild Charge",
    .type = Type::ELECTRIC,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 528,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamageWithRecoil(myMove, opponentMove, 0.25f);
    }
};

const Move MOVE_THUNDERBOLT = {
    .name = "Thunderbolt",
    .type = Type::ELECTRIC,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::PARALYZE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 85,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_ENERGY_BALL = {
    .name = "Energy Ball",
    .type = Type::GRASS,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::SP_DEFENSE_CHANGE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 412,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_LEAF_BLADE = {
    .name = "Leaf Blade",
    .type = Type::GRASS,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 1,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 348,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_ICE_BEAM = {
    .name = "Ice Beam",
    .type = Type::ICE,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::FREEZE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 58,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_ICE_PUNCH = {
    .name = "Ice Punch",
    .type = Type::ICE,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 75,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::FREEZE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 8,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_SLUDGE_BOMB = {
    .name = "Sludge Bomb",
    .type = Type::POISON,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::POISON,
    .secondaryEffectChance = 30.0f,
    .secondaryEffectValue = -1,
    .id = 188,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_POISON_JAB = {
    .name = "Poison Jab",
    .type = Type::POISON,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 32,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::POISON,
    .secondaryEffectChance = 30.0f,
    .secondaryEffectValue = -1,
    .id = 398,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_EARTHQUAKE = {
    .name = "Earthquake",
    .type = Type::GROUND,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 100,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 89,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_EARTH_POWER = {
    .name = "Earth Power",
    .type = Type::GROUND,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::SP_DEFENSE_CHANGE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 414,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_AIR_SLASH = {
    .name = "Air Slash",
    .type = Type::FLYING,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 75,
    .accuracy = 95,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::FLINCH,
    .secondaryEffectChance = 30.0f,
    .secondaryEffectValue = -1,
    .id = 403,

    .protect = true,
    .mirrorMove = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_BRAVE_BIRD = {
    .name = "Brave Bird",
    .type = Type::FLYING,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 120,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 413,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamageWithRecoil(myMove, opponentMove, 1.0f / 3.0f);
    }
};

const Move MOVE_PSYCHIC = {
    .name = "Psychic",
    .type = Type::PSYCHIC,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::SP_DEFENSE_CHANGE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 94,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_PSYCHO_CUT = {
    .name = "Psycho Cut",
    .type = Type::PSYCHIC,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 70,
    .accuracy = 100,
    .maxPP = 32,
    .priority = 0,
    .critRatio = 1,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 427,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_X_SCISSOR = {
    .name = "X-Scissor",
    .type = Type::BUG,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 404,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_BUG_BUZZ = {
    .name = "Bug Buzz",
    .type = Type::BUG,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 90,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::SP_DEFENSE_CHANGE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 405,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,
    .soundBased = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_STONE_EDGE = {
    .name = "Stone Edge",
    .type = Type::ROCK,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 100,
    .accuracy = 80,
    .maxPP = 8,
    .priority = 0,
    .critRatio = 1,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 444,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_POWER_GEM = {
    .name = "Power Gem",
    .type = Type::ROCK,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 32,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 408,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_SHADOW_BALL = {
    .name = "Shadow Ball",
    .type = Type::GHOST,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::SP_DEFENSE_CHANGE,
    .secondaryEffectChance = 20.0f,
    .secondaryEffectValue = -1,
    .id = 247,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_SHADOW_CLAW = {
    .name = "Shadow Claw",
    .type = Type::GHOST,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 70,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 1,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 421,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_DRAGON_CLAW = {
    .name = "Dragon Claw",
    .type = Type::DRAGON,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 337,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_DRAGON_PULSE = {
    .name = "Dragon Pulse",
    .type = Type::DRAGON,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 85,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 406,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_DARK_PULSE = {
    .name = "Dark Pulse",
    .type = Type::DARK,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::FLINCH,
    .secondaryEffectChance = 20.0f,
    .secondaryEffectValue = -1,
    .id = 399,

    .protect = true,
    .mirrorMove = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_NIGHT_SLASH = {
    .name = "Night Slash",
    .type = Type::DARK,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 70,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 1,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 400,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_METEOR_MASH = {
    .name = "Meteor Mash",
    .type = Type::STEEL,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 90,
    .accuracy = 90,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 309,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
        int rand = myMove->battle->randInt(1,11);
        if (rand <= 2){
            MoveFunctions::changeStatModifier(Stat::ATTACK, 1, myMove->user, myMove->battle, myMove);
        }
    }
};

const Move MOVE_FLASH_CANNON = {
    .name = "Flash Cannon",
    .type = Type::STEEL,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 80,
    .accuracy = 100,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::SP_DEFENSE_CHANGE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 430,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_MOONBLAST = {
    .name = "Moonblast",
    .type = Type::FAIRY,
    .damageCategory = DamageCategory::SPECIAL,
    .power = 95,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::SP_ATTACK_CHANGE,
    .secondaryEffectChance = 30.0f,
    .secondaryEffectValue = -1,
    .id = 585,

    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_PLAY_ROUGH = {
    .name = "Play Rough",
    .type = Type::FAIRY,
    .damageCategory = DamageCategory::PHYSICAL,
    .power = 90,
    .accuracy = 90,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::ATTACK_CHANGE,
    .secondaryEffectChance = 10.0f,
    .secondaryEffectValue = -1,
    .id = 583,

    .contact = true,
    .protect = true,
    .mirrorMove = true,
    .kingsRock = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::dealDirectDamage(myMove, opponentMove);
    }
};

const Move MOVE_TOXIC = {
    .name = "Toxic",
    .type = Type::POISON,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 90,
    .maxPP = 16,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 92,
    
    .protect = true,
    .magicCoat = true,
    .mirrorMove = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::applyStatus(&STATUS_BAD_POISON, myMove);
    }
};

const Move MOVE_SPORE = {
    .name = "Spore",
    .type = Type::GRASS,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 100,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 147,
    
    .protect = true,
    .magicCoat = true,
    .mirrorMove = true,
    .sporeBased = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        if (myMove->target->isType(Type::GRASS)){
            myMove->battle->logMessage("It doesn't affect " + myMove->target->nickname + "...");
        }
        else{
            MoveFunctions::applyStatus(&STATUS_SLEEP, myMove);
        }
    }
};

const Move MOVE_THUNDER_WAVE = {
    .name = "Thunder Wave",
    .type = Type::ELECTRIC,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 90,
    .maxPP = 32,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 86,
    
    .protect = true,
    .magicCoat = true,
    .mirrorMove = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::applyStatus(&STATUS_PARALYSIS, myMove);
    }
};

const Move MOVE_WILL_O_WISP = {
    .name = "Will-O-Wisp",
    .type = Type::FIRE,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 85,
    .maxPP = 24,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 261,
    
    .protect = true,
    .magicCoat = true,
    .mirrorMove = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::applyStatus(&STATUS_BURN, myMove);
    }
};

const Move MOVE_POISON_POWDER = {
    .name = "Poison Powder",
    .type = Type::POISON,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 75,
    .maxPP = 56,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 77,
    
    .protect = true,
    .magicCoat = true,
    .mirrorMove = true,
    .sporeBased = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::applyStatus(&STATUS_POISON, myMove);
    }
};

const Move MOVE_POISON_GAS = {
    .name = "Poison Gas",
    .type = Type::POISON,
    .damageCategory = DamageCategory::STATUS,
    .power = 0,
    .accuracy = 90,
    .maxPP = 64,
    .priority = 0,
    .critRatio = 0,
    .targetType = TargetType::OPPONENT,
    .secondaryEffect = SecondaryEffect::NONE,
    .secondaryEffectChance = -1,
    .secondaryEffectValue = -1,
    .id = 139,
    
    .protect = true,
    .magicCoat = true,
    .mirrorMove = true,

    .afterChecks = [](MoveUse* myMove, MoveUse* opponentMove){
        MoveFunctions::applyStatus(&STATUS_POISON, myMove);
    }
};

const std::unordered_map<std::string, const Move*> moves = {
    {MOVE_NONE.name, &MOVE_NONE},
    {MOVE_POUND.name, &MOVE_POUND},
    {MOVE_TACKLE.name, &MOVE_TACKLE},
    {MOVE_KARATE_CHOP.name, &MOVE_KARATE_CHOP},
    {MOVE_SURF.name, &MOVE_SURF},
    {MOVE_FLAMETHROWER.name, &MOVE_FLAMETHROWER},
    {MOVE_HYPER_VOICE.name, &MOVE_HYPER_VOICE},
    {MOVE_RETURN.name, &MOVE_RETURN},
    {MOVE_CLOSE_COMBAT.name, &MOVE_CLOSE_COMBAT},
    {MOVE_AURA_SPHERE.name, &MOVE_AURA_SPHERE},
    {MOVE_FLARE_BLITZ.name, &MOVE_FLARE_BLITZ},
    {MOVE_WATERFALL.name, &MOVE_WATERFALL},
    {MOVE_WILD_CHARGE.name, &MOVE_WILD_CHARGE},
    {MOVE_THUNDERBOLT.name, &MOVE_THUNDERBOLT},
    {MOVE_ENERGY_BALL.name, &MOVE_ENERGY_BALL},
    {MOVE_LEAF_BLADE.name, &MOVE_LEAF_BLADE},
    {MOVE_ICE_BEAM.name, &MOVE_ICE_BEAM},
    {MOVE_ICE_PUNCH.name, &MOVE_ICE_PUNCH},
    {MOVE_SLUDGE_BOMB.name, &MOVE_SLUDGE_BOMB},
    {MOVE_POISON_JAB.name, &MOVE_POISON_JAB},
    {MOVE_EARTHQUAKE.name, &MOVE_EARTHQUAKE},
    {MOVE_EARTH_POWER.name, &MOVE_EARTH_POWER},
    {MOVE_AIR_SLASH.name, &MOVE_AIR_SLASH},
    {MOVE_BRAVE_BIRD.name, &MOVE_BRAVE_BIRD},
    {MOVE_PSYCHIC.name, &MOVE_PSYCHIC},
    {MOVE_PSYCHO_CUT.name, &MOVE_PSYCHO_CUT},
    {MOVE_X_SCISSOR.name, &MOVE_X_SCISSOR},
    {MOVE_BUG_BUZZ.name, &MOVE_BUG_BUZZ},
    {MOVE_STONE_EDGE.name, &MOVE_STONE_EDGE},
    {MOVE_POWER_GEM.name, &MOVE_POWER_GEM},
    {MOVE_SHADOW_BALL.name, &MOVE_SHADOW_BALL},
    {MOVE_SHADOW_CLAW.name, &MOVE_SHADOW_CLAW},
    {MOVE_DRAGON_CLAW.name, &MOVE_DRAGON_CLAW},
    {MOVE_DRAGON_PULSE.name, &MOVE_DRAGON_PULSE},
    {MOVE_DARK_PULSE.name, &MOVE_DARK_PULSE},
    {MOVE_NIGHT_SLASH.name, &MOVE_NIGHT_SLASH},
    {MOVE_METEOR_MASH.name, &MOVE_METEOR_MASH},
    {MOVE_FLASH_CANNON.name, &MOVE_FLASH_CANNON},
    {MOVE_MOONBLAST.name, &MOVE_MOONBLAST},
    {MOVE_PLAY_ROUGH.name, &MOVE_PLAY_ROUGH},
    {MOVE_TOXIC.name, &MOVE_TOXIC},
    {MOVE_SPORE.name, &MOVE_SPORE},
    {MOVE_THUNDER_WAVE.name, &MOVE_THUNDER_WAVE},
    {MOVE_WILL_O_WISP.name, &MOVE_WILL_O_WISP},
    {MOVE_POISON_POWDER.name, &MOVE_POISON_POWDER},
    {MOVE_POISON_GAS.name, &MOVE_POISON_GAS}
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