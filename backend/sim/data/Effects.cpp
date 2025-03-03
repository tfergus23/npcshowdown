#include "sim/data/Effects.hpp"
#include "sim/battle/Battle.hpp"

const Effect EFFECT_CONFUSED;

const Effect EFFECT_ROOST = {
    .observer = {
    .initialize = [](Pokemon* subject, Battle* battle, EffectState* state){
        if (!subject->isType(Type::FLYING)) return;
        
        Type otherType = subject->currentType[0] == Type::FLYING ? subject->currentType[1] : subject->currentType[0];

        subject->currentType = {otherType, Type::NONE};
    },
    .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        subject->currentType = subject->species->type;
        subject->removeEffect(&EFFECT_ROOST);
    }
    },
    .name = "Roost",
};