#pragma once
#include "sim/battle/Event.hpp"
#include "sim/battle/EventArgs.hpp"
#include "sim/battle/ObserverState.hpp"
#include "sim/battle/Stat.hpp"

class Pokemon;
class Battle;



class Observer{
public:
    void handleEvent(Event event, Pokemon* subject, Battle* battle, const EventArgs& args) const;
    void (*initialize)(Pokemon*, Battle*) = [](Pokemon*, Battle*){};
    void (*beforeMove)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*endOfTurn)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*priorityEndOfTurn)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*afterMove)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonSwitch)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonEnter)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonDeath)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    void (*onPokemonAttacked)(Pokemon*,Battle*, const EventArgs&) = [](Pokemon* pokemon,Battle* battle, const EventArgs& e){};
    float (*modifySubjectStat)(Stat,Pokemon*) = [](Stat stat, Pokemon* subject){return 1.0f;};
};