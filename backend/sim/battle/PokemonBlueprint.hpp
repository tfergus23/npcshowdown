#pragma once
#include <string>
#include <array>

class PokemonBlueprint{
    public:
        std::string species = "";
        std::string nickname = "";
        int level = -1;
        std::array<std::string,4> moves;
        std::string abilityName = "";
        std::string gender = "";
        std::array<int,6> ivs;
        std::string nature = "";
        std::string itemName = "";
        std::array<int,6> evs;
        bool empty = true;
        PokemonBlueprint();
        PokemonBlueprint(std::string species, int level, std::array<std::string,4> moves, std::string abilityName, std::string gender, std::array<int,6> ivs, std::string nature, std::string itemName, std::array<int,6> evs, std::string nickname);
};