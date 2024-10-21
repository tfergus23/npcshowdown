#include "sim/battle/PokemonBlueprint.hpp"

PokemonBlueprint::PokemonBlueprint() {}
PokemonBlueprint::PokemonBlueprint(std::string species, uint8_t level, std::array<std::string,4> moves, std::string abilityName, std::string gender, std::array<uint8_t,6> ivs, std::string nature, std::string itemName, std::array<uint8_t,6> evs, std::string nickname) :
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
}

PokemonBlueprint::PokemonBlueprint(json json) : 
    species{json["species"].get<std::string>()},
    level{json["level"].get<uint8_t>()},
    moves{json["moves"].get<std::array<std::string,4>>()},
    abilityName{json["abilityName"].get<std::string>()},
    gender{json["gender"].get<std::string>()},
    ivs{json["ivs"].get<std::array<uint8_t,6>>()},
    nature{json["nature"].get<std::string>()},
    itemName{json["itemName"].get<std::string>()},
    evs{json["evs"].get<std::array<uint8_t,6>>()},
    nickname{json["nickname"].get<std::string>()}
{
}

json PokemonBlueprint::toJSON() const{
    json blueprintjson = {
        {"species", species},
        {"level", level},
        {"moves", moves},
        {"abilityName", abilityName},
        {"gender", gender},
        {"ivs", ivs},
        {"nature", nature},
        {"itemName", itemName},
        {"evs", evs},
        {"nickname", nickname}
    };
    return blueprintjson;
}

bool PokemonBlueprint::equals(const PokemonBlueprint& that) const{
    for (int i = 0; i < moves.size(); i++){
        if (moves[i] != that.moves[i]){
            return false;
        }
    }
    for (int i = 0; i < evs.size(); i++){
        if (evs[i] != that.evs[i]){
            return false;
        }
    }
    for (int i = 0; i < ivs.size(); i++){
        if (ivs[i] != that.ivs[i]){
            return false;
        }
    }
    return species == that.species &&
           level == that.level &&
           abilityName == that.abilityName &&
           gender == that.gender &&
           nature == that.nature &&
           itemName == that.itemName &&
           nickname == that.nickname;
}