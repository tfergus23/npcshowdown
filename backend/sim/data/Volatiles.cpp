#include "sim/data/Volatiles.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"

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

const Volatile VOLATILE_LEECH_SEED;