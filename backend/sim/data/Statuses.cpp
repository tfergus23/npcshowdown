#include "sim/data/Statuses.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"
#include "sim/utils/move_functions.hpp"
#include "sim/data/Abilities.hpp"

const Status STATUS_NONE;

//TODO
const Status STATUS_PARALYSIS;

//TODO
const Status STATUS_POISON;

//TODO
const Status STATUS_SLEEP;

//TODO
const Status STATUS_BURN{
    .observer{
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        auto* mu = e.moveUse;
        if (mu->user == subject && mu->move->damageCategory == DamageCategory::PHYSICAL && mu->move != &MOVE_CONFUSION_HIT && !mu->user->hasAbilityUnsuppressed(&ABILITY_GUTS)){
            mu->damageMod *= 0.5f;
        }
    },
    .endOfTurn  = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        battle->log(subject->nickname + " was hurt by it's burn!");
        dealResidualPercentDamage(6.25f, subject,battle);
    }
    },
    .was = " was burned!"
};

//TODO
const Status STATUS_FROZEN;

const Status STATUS_BAD_POISON;