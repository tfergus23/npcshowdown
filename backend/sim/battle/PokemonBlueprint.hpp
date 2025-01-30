#pragma once
#include <string>
#include <array>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class PokemonBlueprint{
    public:
        std::string species = "";
        std::string nickname = "";
        uint8_t level = 0;
        std::array<std::string,4> moves = {"", "", "", ""};
        std::string abilityName = "";
        std::string gender = "";
        std::array<uint8_t,6> ivs = {0,0,0,0,0,0};
        std::string nature = "";
        std::string itemName = "";
        std::array<uint8_t,6> evs = {0,0,0,0,0,0};
        PokemonBlueprint();
        PokemonBlueprint(std::string species, uint8_t level, std::array<std::string,4> moves, std::string abilityName, std::string gender, std::array<uint8_t,6> ivs, std::string nature, std::string itemName, std::array<uint8_t,6> evs, std::string nickname);
        PokemonBlueprint(json json);
        json toJSON() const;
        bool equals(const PokemonBlueprint& that) const;
        size_t hashCode() const;
};