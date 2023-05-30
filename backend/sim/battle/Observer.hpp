#pragma once
#include "sim/battle/Event.hpp"
#include "sim/battle/EventArgs.hpp"
#include "sim/battle/EffectState.hpp"

class Pokemon;
class Battle;



class Observer{
public:
    void handleEvent(Event event, Pokemon* subject, Battle* battle, const EventArgs& args) const;
    void (*initializeState)(EffectState*) = [](EffectState* state){};
//protected:
    void (*beforeMove)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*endOfTurn)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*priorityEndOfTurn)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*afterMove)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonSwitch)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonEnter)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonDeath)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonAttacked)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
};