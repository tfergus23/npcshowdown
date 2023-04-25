#pragma once
#include "sim/battle/Event.hpp"
#include "sim/battle/EventArgs.hpp"
#include "sim/battle/EffectState.hpp"

class Observer{
public:
    void handleEvent(Event event, Pokemon* subject, Battle* battle, const EventArgs& args) const;
    virtual void initializeState(EffectState* state) const {}
protected:
    virtual void beforeMove(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
    virtual void endOfTurn(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
    virtual void priorityEndOfTurn(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
    virtual void afterMove(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
    virtual void onPokemonSwitch(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
    virtual void onPokemonEnter(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
    virtual void onPokemonDeath(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
    virtual void onPokemonAttacked(Pokemon* subject, Battle* battle, const EventArgs& args) const {}
};