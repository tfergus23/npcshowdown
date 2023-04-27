#include "Observer.hpp"
#include "sim/battle/Battle.hpp"

void Observer::handleEvent(Event event, Pokemon* subject, Battle* battle, const EventArgs& args) const {
    switch (event)
    {
    case BEFORE_MOVE:
        battle->assertTrue(args.moveUse != nullptr, "Incorrect args for BEFORE_MOVE event - moveUse is nullptr");
        beforeMove(subject, battle, args);
        break;
    case END_OF_TURN:
        endOfTurn(subject, battle, args);
        break;
    case PRIORITY_END_OF_TURN:
        priorityEndOfTurn(subject, battle, args);
        break;
    case AFTER_MOVE:
        battle->assertTrue(args.moveUse != nullptr, "Incorrect args for AFTER_MOVE event - moveUse is nullptr");
        afterMove(subject, battle, args);
        break;
    case POKEMON_SWITCH:
        battle->assertTrue(args.eventSubject != nullptr, "Incorrect args for POKEMON_SWITCH event - eventSubject is nullptr");
        onPokemonSwitch(subject, battle, args);
        break;
    case POKEMON_ENTER:
        battle->assertTrue(args.eventSubject != nullptr, "Incorrect args for POKEMON_ENTER event - eventSubject is nullptr");
        onPokemonEnter(subject, battle, args);
        break;
    case POKEMON_DEATH:
        battle->assertTrue(args.eventSubject != nullptr, "Incorrect args for POKEMON_DEATH event - eventSubject is nullptr");
        onPokemonDeath(subject, battle, args);
        break;
    case POKEMON_ATTACKED:
        battle->assertTrue(args.moveUse != nullptr, "Incorrect args for POKEMON_ATTACKED event - moveUse is nullptr");
        onPokemonAttacked(subject, battle, args);
        break;

    default:
        break;
    }
}