#pragma once
#include "sim/battle/Pokemon.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/battle/MoveUse.hpp"

class Observer{
public:
    void handleEvent(Event event, Pokemon* subject, Battle* battle, const EventArgs& args) const {
        switch (event)
        {
        case BEFORE_MOVE:
            battle->assert(args.moveUse != nullptr, "Incorrect args for BEFORE_MOVE event - moveUse is nullptr");
            beforeMove(subject,battle,args);
            break;
        case END_OF_TURN:
            endOfTurn(subject,battle,args);
            break;
        case PRIORITY_END_OF_TURN:
            priorityEndOfTurn(subject,battle,args);
            break;
        case AFTER_MOVE:
            battle->assert(args.moveUse != nullptr, "Incorrect args for AFTER_MOVE event - moveUse is nullptr");
            afterMove(subject,battle,args);
            break;
        case POKEMON_SWITCH:
            battle->assert(args.eventSubject != nullptr, "Incorrect args for POKEMON_SWITCH event - eventSubject is nullptr");
            onPokemonSwitch(subject,battle,args);
            break;
        case POKEMON_ENTER:
            battle->assert(args.eventSubject != nullptr, "Incorrect args for POKEMON_ENTER event - eventSubject is nullptr");
            onPokemonEnter(subject,battle,args);
            break;
        case POKEMON_DEATH:
            battle->assert(args.eventSubject != nullptr, "Incorrect args for POKEMON_DEATH event - eventSubject is nullptr");
            onPokemonDeath(subject,battle,args);
            break;
        case POKEMON_ATTACKED:
            battle->assert(args.eventSubject != nullptr, "Incorrect args for POKEMON_ATTACKED event - eventSubject is nullptr");
            onPokemonAttacked(subject,battle,args);
            break;
        
        default:
            break;
        }
    }
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