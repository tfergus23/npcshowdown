#include "sim/data/Volatiles.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"
#include "sim/utils/MoveFunctions.hpp"

const Volatile VOLATILE_NONE;

const Volatile VOLATILE_CONFUSED = {
    .name = "Confused",
    .was = " became confused!"
};

const Volatile VOLATILE_ROOST = {
    .observer = {
    .initialize = [](Pokemon* subject, Battle* battle){
        if (!subject->isType(Type::FLYING)) return;
        
        Type otherType = subject->currentType[0] == Type::FLYING ? subject->currentType[1] : subject->currentType[0];

        subject->currentType = {otherType, Type::NONE};
    },
    .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        subject->currentType = subject->species->type;
        subject->removeVolatile(&VOLATILE_ROOST);
    }
    },
    .name = "Roost",
};

const Volatile VOLATILE_PROTECTED = {
    .observer = {
        .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
            if (e.moveUse->target == subject && e.moveUse->move->protect){
                e.moveUse->fail(subject->nickname + " protected itself!");
            }
        },

        .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
            subject->removeVolatile(&VOLATILE_PROTECTED);
        }
    },
    .name = "Protected"
};

const Volatile VOLATILE_PROTECT_STATE = {
    .observer{
        .initialize = [](Pokemon* subject,  Battle* battle){
            subject->initializeVolatileState<ProtectState>(&VOLATILE_PROTECT_STATE);
        },

        .afterMove = [](Pokemon* subject, Battle* battle, const EventArgs&e){
            if (e.moveUse->user == subject && e.moveUse->move != &MOVE_PROTECT){
                subject->getVolatileState<ProtectState>(&VOLATILE_PROTECT_STATE).protectsInARow = 0;
            }
        }
    }
};

const Volatile VOLATILE_LEECH_SEED = {
.observer = {
    .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        Pokemon* opponent = subject == battle->player1ActivePokemon ? battle->player2ActivePokemon : battle->player1ActivePokemon;
        int damage = MoveFunctions::dealResidualPercentDamage(((1.0f / 8.0f)*100.0f), subject, battle);
        if (damage > 0){
            battle->logDamageTaken(subject->nickname + " had it's health drained!", {.recipientIsPlayer1 = (subject == battle->player1ActivePokemon), .damage = damage});
            MoveFunctions::giveFlatHealing(damage, opponent, battle);
        }
    }
},
    .name = "Leech Seed",
    .was = " was seeded!"
};

const Volatile VOLATILE_FLYING = {
.observer = {
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        if (e.moveUse->target == subject && e.moveUse->move->targetType == TargetType::OPPONENT && !e.moveUse->move->hitsFly){
            e.moveUse->fail(e.moveUse->target->nickname + "'s attack missed!");
        }
    }
},
    .name = "Flying"
};

const Volatile VOLATILE_DIGGING = {
.observer = {
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        if (e.moveUse->target == subject && e.moveUse->move->targetType == TargetType::OPPONENT && !e.moveUse->move->hitsDig){
            e.moveUse->fail(e.moveUse->target->nickname + "'s attack missed!");
        }
    }
},
    .name = "Digging"
};