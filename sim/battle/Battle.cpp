#include "battle/Battle.hpp"
#include <random>
#define DEBUG_LOG 1

Battle::Battle(const Trainer& trainer1, const Trainer& trainer2, int seed) :
m_Player1{trainer1},
m_Player2{trainer2},
m_Seed{seed}
{
    log("Seed: " + std::to_string(seed));
    for (int i = 0; i < m_Player1.getTeamBlueprint().size(); i++){
        const PokemonBlueprint* blueprint = &m_Player1.getTeamBlueprint()[i];
        if (!blueprint->empty){
            player1Team[i] = Pokemon(blueprint, this);
        }
    }
}

void Battle::log(const std::string& message){
    battleLog += message + "\n";
}

void Battle::debug(const std::string& message){
#if DEBUG_LOG
    battleLog += message + "\n";
#endif
}



int Battle::randInt(int min, int max){
    int rand = m_Distribution(m_Generator);
    int diff = max - min;
    int offset = rand % diff;
    return min + offset;
}

int Battle::getSeed(){
    return m_Seed;
}