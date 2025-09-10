#include "sim/data/Statuses.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"
#include "sim/utils/move_functions.hpp"
#include "sim/data/Abilities.hpp"

const Status STATUS_NONE{
    .name = "None"
};

//TODO
const Status STATUS_PARALYSIS {
    .observer{
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        if (e.moveUse->user == subject){
            int rand = battle->randInt(1,101);
            if (rand <= 25){
                e.moveUse->dontStart(subject->nickname + " is paralyzed! It can't move!");
            }
        }
    },
    .modifySubjectStat = [](Stat stat, Pokemon* subject){
        if (stat == Stat::SPEED){
            return 0.5f;
        }
        return 1.0f;
    }
    },
    .was = " is paralyzed! It may be unable to move!",
    .name = "Paralysis"
};

//TODO
const Status STATUS_POISON{
    .observer{
    .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        int dmg = dealResidualPercentDamage(12.5f, subject, battle);
        battle->logDamageTaken(subject->nickname + " is hurt by poison!", {.recipientIsPlayer1 = subject == battle->player1ActivePokemon, .damage = dmg});
    }
    },
    .was = " was poisoned!",
    .name = "Poison"
};

//TODO
const Status STATUS_SLEEP{
    .observer{
    .initialize = [](Pokemon* subject, Battle* battle){
        SleepState& state =  std::get<SleepState>(subject->abilityState);
        state.remainingTurns = battle->randInt(1,4);
    },
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        if (e.moveUse->user != subject) return;
        SleepState& state =  std::get<SleepState>(subject->abilityState);
        if (state.remainingTurns > 0){
            if (e.moveUse->move->usableWhileAsleep){
                battle->logMessage(subject->nickname + " is fast asleep.");
            }
            else{
                e.moveUse->dontStart(subject->nickname + " is fast asleep.");
            }
        }
        else{
            subject->applyStatus(&STATUS_NONE);
            battle->logApplyStatus(subject->nickname + " woke up!", {.appliedToPlayer1 = subject == battle->player1ActivePokemon, .status = &STATUS_NONE});
        }
        state.remainingTurns--;
    }
    },
    .was = " fell asleep!",
    .name = "Sleep"
};

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
        int dmg = dealResidualPercentDamage(6.25f, subject,battle);
        battle->logDamageTaken(subject->nickname + " was hurt by it's burn!", {.recipientIsPlayer1 = subject == battle->player1ActivePokemon, .damage = dmg});
    }
    },
    .was = " was burned!",
    .name = "Burn"
};

//TODO
const Status STATUS_FROZEN{
    .observer{
    .beforeMove  = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        if (e.moveUse->user != subject) return;

        if (battle->randInt(1,11) <= 2 || (e.moveUse->move->type == Type::FIRE && e.moveUse->move->damageCategory != DamageCategory::STATUS)){
            subject->applyStatus(&STATUS_NONE);
            battle->logApplyStatus(subject->nickname + " thawed out!", {.appliedToPlayer1 = subject == battle->player1ActivePokemon, .status = &STATUS_NONE});
        }
        else{
            e.moveUse->dontStart(subject->nickname + " is frozen solid!");
        }
    }
    },
    .was = " is frozen solid!",
    .name = "Frozen"
};

const Status STATUS_BAD_POISON{
    .observer{
    .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        BadPoisonState& state = std::get<BadPoisonState>(subject->abilityState);
        state.activeTurns++;
        float percentDamage = 100.0f * (state.activeTurns / 16.0f);
        int dmg = dealResidualPercentDamage(percentDamage, subject, battle);
        battle->logDamageTaken(subject->nickname + " is hurt by poison!", {.recipientIsPlayer1 = subject == battle->player1ActivePokemon, .damage = dmg});
        
    },
    .onPokemonSwitch = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        if (e.eventSubject == subject){
            BadPoisonState& state = std::get<BadPoisonState>(subject->abilityState);
            state.activeTurns = 0;
        }
    }
    },
    .was = " was badly poisoned!",
    .name = "Toxic"
};