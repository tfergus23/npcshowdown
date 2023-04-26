#include <iostream>

#include "sim/battle/Battle.hpp"
#include "api/NPCS_API_Server.hpp"
#include <chrono>
#define STRESS_TEST 0



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
    std::array<PokemonBlueprint, 6> trainer1Team = {poke1, poke1, poke1, poke1, poke1 ,poke1 };
    std::array<PokemonBlueprint, 6> trainer2Team = {poke2, poke2, poke2, poke2, poke2 ,poke2 };

    Trainer trainer1("Youngster", "Joey", trainer1Team, WILD);
    Trainer trainer2("Youngster", "Ben", trainer2Team, WILD);

#if STRESS_TEST
    auto start = std::chrono::high_resolution_clock::now();
    int battlesRan = 0;
    std::vector<Trainer*> winners;
    while (battlesRan < 20000) {
        Battle battle(&trainer1, &trainer2, rand());
        battle.doLogging = false;
        simulateBattle(&battle);
        winners.push_back(battle.winner);
        battlesRan++;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0f << '\n';
    std::cout << winners[rand() % 20000]->getFullName() << "\n";
#else

    
    Battle battle(&trainer1, &trainer2, rand());
    simulateBattle(&battle);
    std::cout << battle.battleLog << '\n';
    
#endif
   /*
    NPCS_API_Server server;
    return server.run();
   */
}