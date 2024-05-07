#include "sim/battle/Trainer.hpp"
#include <iostream>
#include "sim/data/Moves.hpp"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

std::vector<PokemonBlueprint> teamBluePrintFromJSON(const json& json){
    std::vector<PokemonBlueprint> teamBlueprint;
    for(int i = 0; i < json.size(); i++){
        teamBlueprint.push_back(PokemonBlueprint(json[i]));
    }
    return teamBlueprint;
}

Trainer::Trainer(const std::string& name, const std::vector<PokemonBlueprint>& teamBlueprint, TrainerLevel trainerLevel) : 
teamBlueprint{teamBlueprint},
trainerInfo{TrainerInfo(name, trainerLevel)}
{
}

Trainer::Trainer(json json) :
//teamBlueprint{{PokemonBlueprint(json["team"][0]),PokemonBlueprint(json["team"][1]),PokemonBlueprint(json["team"][2]),PokemonBlueprint(json["team"][3]),PokemonBlueprint(json["team"][4]),PokemonBlueprint(json["team"][5])}},
teamBlueprint{teamBluePrintFromJSON(json["team"])},
trainerInfo{TrainerInfo(json["name"].get<std::string>(), trainerLevelFromString(json["trainerLevel"].get<std::string>()))}
{
}


std::string Trainer::getFullName() const{
    return trainerInfo.name;
}

json Trainer::toJSON() const{
    std::vector<json> teamJSON;
    for(auto& poke: teamBlueprint){
        teamJSON.push_back(poke.toJSON());
    }
    json json = {
        {"name", trainerInfo.name},
        {"trainerLevel", stringFromTrainerLevel(trainerInfo.trainerLevel)},
        {"team", teamJSON}
    };
    return json;
}

bool Trainer::equals(const Trainer& that) const{
    for (int i = 0; i < teamBlueprint.size(); i++){
        if (!teamBlueprint[i].equals(that.teamBlueprint[i])){
            return false;
        }
    }
    return trainerInfo.name == that.trainerInfo.name &&
           trainerInfo.trainerLevel == that.trainerInfo.trainerLevel;
}