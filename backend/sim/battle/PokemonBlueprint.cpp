#include "sim/battle/PokemonBlueprint.hpp"

PokemonBlueprint::PokemonBlueprint(std::string species, int level, std::array<std::string,4> moves, std::string abilityName, std::string gender, std::array<int,6> ivs, std::string nature, std::string itemName, std::array<int,6> evs, std::string nickname) :
    species{species},
    level{level},
    moves{moves},
    abilityName{abilityName},
    gender{gender},
    ivs{ivs},
    nature{nature},
    itemName{itemName},
    evs{evs},
    nickname{nickname}
{
    empty = false;
}