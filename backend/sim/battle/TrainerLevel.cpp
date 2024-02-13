#include "TrainerLevel.hpp"
#include <unordered_map>
#include <string>
#include <vector>
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

TrainerLevel trainerLevelFromString(const std::string& string){
    return stringToLevelMap.at(string);
}
std::string stringFromTrainerLevel(TrainerLevel level){
    return levelToStringMap.at(level);
}

std::string createTrainerLevelDataResponse(){
    std::vector<std::string> levelNames;
    for (auto [name,ptr] : stringToLevelMap){
        levelNames.push_back(name);
    }
    std::sort(levelNames.begin(), levelNames.end());
    json response;
    response["success"] = true;
    response["message"] = "OK";
    response["data"] = levelNames;
    return response.dump();
}