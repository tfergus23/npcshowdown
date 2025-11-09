#include "sim/data/FieldEffects.hpp"
#include "sim/battle/Battle.hpp"

const FieldEffect FIELD_EFFECT_NONE;
const FieldEffect FIELD_EFFECT_SUBSTITUTE;

const FieldEffect FIELD_EFFECT_REFLECT = {
.observer = {
    .initialize = [](Pokemon* subject, Battle* battle){
        bool side = subject == battle->player1ActivePokemon;
        battle->initializeFieldEffectState<ScreenState>(side, &FIELD_EFFECT_REFLECT);
    },
    .beforeMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        if (e.moveUse->target == subject && e.moveUse->move->damageCategory == DamageCategory::PHYSICAL){
            battle->logMessage("reflect active");
            e.moveUse->damageMod *= (2732.0f/4096.0f); // This is the actual constant listed on bulbapedia...
        }
    },

    .afterMove = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        bool side = subject == battle->player1ActivePokemon;
        if (e.moveUse->target == subject && e.moveUse->move->breaksScreen){
            battle->removeFieldEffect(side, &FIELD_EFFECT_REFLECT);
            battle->logRemoveFieldEffect("The reflect was shattered!", {.removedFromPlayer1Side = side, .fieldEffect = &FIELD_EFFECT_REFLECT});
        }
    },

    .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& e){
        bool side = subject == battle->player1ActivePokemon;
        auto& state = battle->getFieldEffectState<ScreenState>(side, &FIELD_EFFECT_REFLECT);
        state.remainingTurns--;
        if (state.remainingTurns == 0){
            battle->removeFieldEffect(side, &FIELD_EFFECT_REFLECT);
            battle->logRemoveFieldEffect("The reflect faded!", {.removedFromPlayer1Side = side, .fieldEffect = &FIELD_EFFECT_REFLECT});
        }
    }
},
    .name = "Reflect"
};