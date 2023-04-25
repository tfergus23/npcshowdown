#include <iostream>

#include "sim/Sim.hpp"
#include "sim/battle/Battle.hpp"
#include "api/NPCS_API_Server.hpp"

int main(){
    
    PokemonBlueprint poke1(
        "Squirtle",
        100,
        {"Pound", "", "", ""},
        "Guts",
        "Male",
        {31,31,31,31,31,31},
        "Serious",
        "",
        {252,0,4,252,0,0},
        "Squirtle"
    );
    PokemonBlueprint poke2(
        "Charmander",
        100,
        {"Pound", "", "", ""},
        "Guts",
        "Male",
        {31,31,31,31,31,31},
        "Serious",
        "",
        {252,0,4,252,0,0},
        "Charmander"
    );
    std::array<PokemonBlueprint, 6> trainer1Team = {poke1, PokemonBlueprint(), PokemonBlueprint(), PokemonBlueprint(), PokemonBlueprint() ,PokemonBlueprint()};
    std::array<PokemonBlueprint, 6> trainer2Team = {poke2, PokemonBlueprint(), PokemonBlueprint(), PokemonBlueprint(), PokemonBlueprint() ,PokemonBlueprint()};

    Trainer trainer1("Youngster", "Joey", trainer1Team, WILD);
    Trainer trainer2("Youngster", "Ben", trainer2Team, WILD);

    Battle battle(trainer1, trainer2, 69);
    simulateBattle(&battle);

    std::cout << battle.battleLog << '\n';
    

   /*
    NPCS_API_Server server;
    return server.run();
   */
}