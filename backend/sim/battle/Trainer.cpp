#include "sim/battle/Trainer.hpp"
#include <iostream>
#include "sim/data/Moves.hpp"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

const std::unordered_map<std::string, TrainerLevel> stringToLevelMap = {
    {"First Move", TrainerLevel::FIRST_MOVE},
    {"Use Two Moves then Switch", TrainerLevel::USE_2_MOVES_THEN_SWITCH},
    {"Wild", TrainerLevel::WILD},
    {"Switcher", TrainerLevel::SWITCHER},
    {"Trainer", TrainerLevel::TRAINER},
    {"Boss", TrainerLevel::BOSS}
};

const std::unordered_map<TrainerLevel, std::string> levelToStringMap = {
    {TrainerLevel::FIRST_MOVE, "First Move"},
    {TrainerLevel::USE_2_MOVES_THEN_SWITCH, "Use Two Moves then Switch"},
    {TrainerLevel::WILD, "Wild"},
    {TrainerLevel::SWITCHER, "Switcher"},
    {TrainerLevel::TRAINER, "Trainer"},
    {TrainerLevel::BOSS, "Boss"}
};



Trainer::Trainer(std::string_view trainerClass, std::string_view name, const std::vector<PokemonBlueprint>& teamBlueprint, TrainerLevel trainerLevel) : 
teamBlueprint{teamBlueprint},
trainerClass{trainerClass},
name{name},
trainerLevel{trainerLevel}
{
}

Trainer::Trainer(json json) :
//teamBlueprint{{PokemonBlueprint(json["team"][0]),PokemonBlueprint(json["team"][1]),PokemonBlueprint(json["team"][2]),PokemonBlueprint(json["team"][3]),PokemonBlueprint(json["team"][4]),PokemonBlueprint(json["team"][5])}},
trainerClass{json["trainerClass"].get<std::string>()},
name{json["name"].get<std::string>()},
trainerLevel{stringToLevelMap.at(json["trainerLevel"].get<std::string>())}
{
    for(int i = 0; i < json["team"].size(); i++){
        teamBlueprint.push_back(PokemonBlueprint(json["team"][i]));
    }
}


std::string Trainer::getFullName() const{
    if (trainerClass == ""){
        return name;
    }
    return trainerClass + " " + name;
}

const Move* Trainer::pickMove(Pokemon* myPoke, Pokemon* enemyPoke, Battle* battle){
    tflib::static_vector<const Move*, 4> validMoves;
    bool isPlayer1 = this == battle->getPlayer1();
    auto& myTeam = isPlayer1 ?  battle->player1Team : battle->player2Team;
    int& switchCounter = isPlayer1 ? battle->player1SwitchCounter : battle->player2SwitchCounter;
    for (int i = 0; i < myPoke->currentMoves.size(); i++){
        if (myPoke->currentMoves[i] != MOVE_NONE && myPoke->currentPP[i] > 0 && i != myPoke->disabledIndex){
            validMoves.push_back(myPoke->currentMoves[i]);
        }
    }


    //TODO: The rest of these
    switch (trainerLevel)
    {
    case FIRST_MOVE:
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[0];
    case SWITCHER:
        if (getValidSwitchesCount(myPoke, battle) > 0){
            return &MOVE_SWITCH;
        }
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[battle->randInt(0, validMoves.size())];
    case WILD:
        if (validMoves.size() <= 0){
            return &MOVE_STRUGGLE;
        }
        return validMoves[battle->randInt(0, validMoves.size())];
    case USE_2_MOVES_THEN_SWITCH:
        if (switchCounter > 2){
            if (getValidSwitchesCount(myPoke, battle) > 0){
                switchCounter = 0;
                return &MOVE_SWITCH;
            }
            else{
                if (validMoves.size() <= 0){
                    return &MOVE_STRUGGLE;
                }
                return validMoves[battle->randInt(0, validMoves.size())];
            }
        }
        else{
            switchCounter++;
            if (validMoves.size() <= 0){
                return &MOVE_STRUGGLE;
            }
            return validMoves[battle->randInt(0, validMoves.size())];
        }
    case TRAINER:
        std::cerr << "Unhandled trainer level.\n";
        throw 1;
    case BOSS:
        std::cerr << "Unhandled trainer level.\n";
        throw 1;
    default:
        std::cerr << "Unhandled trainer level.\n";
        throw 1;
    }
    return nullptr;
}

int Trainer::pickPokemon(Pokemon* currentlyActivePokemon, Pokemon* enemyPoke, Battle* battle){
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    tflib::static_vector<int,5> validSlots;
    getValidSwitches(currentlyActivePokemon, battle, validSlots);
    battle->assertTrue(validSlots.size() > 0, "pickPokemon called without any valid pokemon to switch to.");
    return validSlots[battle->randInt(0, validSlots.size())];
}

void Trainer::getValidSwitches(Pokemon* currentlyActivePokemon, Battle* battle, tflib::static_vector<int,5>& outVec){
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    if (currentlyActivePokemon->isTrapped && !currentlyActivePokemon->isDead) return;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            outVec.push_back(i);
        }
    }
}

int Trainer::getValidSwitchesCount(Pokemon* currentlyActivePokemon, Battle* battle){
    auto& myTeam = this == battle->getPlayer1() ? battle->player1Team : battle->player2Team;
    if (currentlyActivePokemon->isTrapped && !currentlyActivePokemon->isDead) return 0;
    int result = 0;
    for (int i = 0; i < myTeam.size(); i++){
        if (!myTeam[i].empty && !myTeam[i].isDead && currentlyActivePokemon != &(myTeam[i])){
            result++;
        }
    }
    return result;
}

json Trainer::toJSON() const{
    json json = {
        {"name", name},
        {"trainerClass", trainerClass},
        {"trainerLevel", levelToStringMap.at(trainerLevel)},
        {"team", {teamBlueprint[0].toJSON(),teamBlueprint[1].toJSON(),teamBlueprint[2].toJSON(),teamBlueprint[3].toJSON(),teamBlueprint[4].toJSON(),teamBlueprint[5].toJSON()}}
    };
    return json;
}

bool Trainer::equals(const Trainer& that) const{
    for (int i = 0; i < teamBlueprint.size(); i++){
        if (!teamBlueprint[i].equals(that.teamBlueprint[i])){
            return false;
        }
    }
    return name == that.name &&
           trainerClass == that.trainerClass &&
           trainerLevel == that.trainerLevel;
}

TrainerLevel Trainer::levelFromString(const std::string& string){
    return stringToLevelMap.at(string);
}