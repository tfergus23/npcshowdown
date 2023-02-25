#pragma once
#include <string>
#include <array>

class PokemonBlueprint{
    public:
        const std::string species;
        const std::string nickname;
        const int level;
        const std::array<int,4> moves;
        const std::string abilityName;
        const std::string gender;
        const std::array<int,6> ivs;
        const std::string nature;
        const std::string itemName;
        const std::array<int,6> evs;
        PokemonBlueprint(std::string species, int level, std::array<int,4> moves, std::string abilityName, std::string gender, std::array<int,6> ivs, std::string nature, std::string itemName, std::array<int,6> evs, std::string nickname);
};