#include "sim/battle/PokemonBlueprint.hpp"
#include <functional>

PokemonBlueprint::PokemonBlueprint() {}
PokemonBlueprint::PokemonBlueprint(std::string species, uint16_t level, std::array<std::string,4> moves, std::string abilityName, std::string gender, std::array<uint8_t,6> ivs, std::string nature, std::string itemName, std::array<uint8_t,6> evs, std::string nickname) :
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
    level{json["level"].get<uint16_t>()},
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

size_t PokemonBlueprint::hashCode() const{
    size_t seed = 0;

    // Combine the hashes of all the fields
    auto hash_combine = [&seed](const auto& value) {
        std::hash<std::decay_t<decltype(value)>> hash_fn;
        seed ^= hash_fn(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2); //0x9e3779b9 is a 'golden ratio' constant
    };

    hash_combine(species);
    hash_combine(nickname);
    hash_combine(level);
    hash_combine(abilityName);
    hash_combine(gender);
    hash_combine(nature);
    hash_combine(itemName);

    // Handle arrays: hash each element
    for (const auto& move : moves) {
        hash_combine(move);  // Hash each string in the moves array
    }

    for (const auto& iv : ivs) {
        hash_combine(iv);    // Hash each value in the ivs array
    }

    for (const auto& ev : evs) {
        hash_combine(ev);    // Hash each value in the evs array
    }

    return seed;
}