#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"

int main(){
    PokemonBlueprint blastoise(
        "Blastoise",
        100,
        {"Surf","","",""},
        "Torrent",
        "Male",
        {31,31,31,31,31,31},
        "Modest",
        "",
        {252,0,0,252,0,0},
        ""
    );
    PokemonBlueprint snorlax(
        "Snorlax",
        100,
        {"Tackle","","",""},
        "Guts",
        "Male",
        {31,31,31,31,31,31},
        "Careful",
        "",
        {252,0,152,0,104,0},
        ""
    );
    Trainer t1("test1", {blastoise}, TrainerLevel::WILD);
    Trainer t2("test2", {snorlax}, TrainerLevel::WILD);


    Battle battle(t1,t2,0, {.averageDamage = true});
    battle.addMoves(&MOVE_SURF, &MOVE_TACKLE);
    battle.doMove();
    int damage = battle.m_Turn[0].damageDone;

    assert(damage == 102);
    return 0;
}