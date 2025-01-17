#include "sim/battle/Nature.hpp"
#include "sim/battle/Stat.hpp"
#include <vector>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

static const std::unordered_map<std::string, Nature> stringToNature = {
    {"Hardy", Nature::HARDY},
    {"Lonely", Nature::LONELY},
    {"Brave", Nature::BRAVE},
    {"Adamant", Nature::ADAMANT},
    {"Naughty", Nature::NAUGHTY},
    {"Bold", Nature::BOLD},
    {"Docile", Nature::DOCILE},
    {"Relaxed", Nature::RELAXED},
    {"Impish", Nature::IMPISH},
    {"Lax", Nature::LAX},
    {"Timid", Nature::TIMID},
    {"Hasty", Nature::HASTY},
    {"Serious", Nature::SERIOUS},
    {"Jolly", Nature::JOLLY},
    {"Modest", Nature::MODEST},
    {"Mild", Nature::MILD},
    {"Quiet", Nature::QUIET},
    {"Bashful", Nature::BASHFUL},
    {"Rash", Nature::RASH},
    {"Calm", Nature::CALM},
    {"Gentle", Nature::GENTLE},
    {"Sassy", Nature::SASSY},
    {"Careful", Nature::CAREFUL},
    {"Quirky", Nature::QUIRKY}
};

static const std::unordered_map<Nature, std::string> natureToString = {
    {Nature::HARDY, "Hardy"},
    {Nature::LONELY, "Lonely"},
    {Nature::BRAVE, "Brave"},
    {Nature::ADAMANT, "Adamant"},
    {Nature::NAUGHTY, "Naughty"},
    {Nature::BOLD, "Bold"},
    {Nature::DOCILE, "Docile"},
    {Nature::RELAXED, "Relaxed"},
    {Nature::IMPISH, "Impish"},
    {Nature::LAX, "Lax"},
    {Nature::TIMID, "Timid"},
    {Nature::HASTY, "Hasty"},
    {Nature::SERIOUS, "Serious"},
    {Nature::JOLLY, "Jolly"},
    {Nature::MODEST, "Modest"},
    {Nature::MILD, "Mild"},
    {Nature::QUIET, "Quiet"},
    {Nature::BASHFUL, "Bashful"},
    {Nature::RASH, "Rash"},
    {Nature::CALM, "Calm"},
    {Nature::GENTLE, "Gentle"},
    {Nature::SASSY, "Sassy"},
    {Nature::CAREFUL, "Careful"},
    {Nature::QUIRKY, "Quirky"},
};


static const float natureChart[25][5] = {
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.1f,0.9f,1.0f,1.0f,1.0f},
    {1.1f,1.0f,1.0f,1.0f,0.9f},
    {1.1f,1.0f,0.9f,1.0f,1.0f},
    {1.1f,1.0f,1.0f,0.9f,1.0f},
    {0.9f,1.1f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.1f,1.0f,1.0f,0.9f},
    {1.0f,1.1f,0.9f,1.0f,1.0f},
    {1.0f,1.1f,1.0f,0.9f,1.0f},
    {0.9f,1.0f,1.0f,1.0f,1.1f},
    {1.0f,0.9f,1.0f,1.0f,1.1f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,0.9f,1.0f,1.1f},
    {1.0f,1.0f,1.0f,0.9f,1.1f},
    {0.9f,1.0f,1.1f,1.0f,1.0f},
    {1.0f,0.9f,1.1f,1.0f,1.0f},
    {1.0f,1.0f,1.1f,1.0f,0.9f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.1f,0.9f,1.0f},
    {0.9f,1.0f,1.0f,1.1f,1.0f},
    {1.0f,0.9f,1.0f,1.1f,1.0f},
    {1.0f,1.0f,1.0f,1.1f,0.9f},
    {1.0f,1.0f,0.9f,1.1f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f}
};

float natureBoost(Nature nature, Stat stat){
    int natureIndex = (int) nature;
    int statIndex = (int) stat - 1;
    return natureChart[natureIndex][statIndex];
}

std::string createNatureDataResponse(){
    std::vector<std::string> natureNames;
    for (auto [name,ptr] : stringToNature){
        if (name != ""){
            natureNames.push_back(name);
        }
    }
    std::sort(natureNames.begin(), natureNames.end());
    json response;
    response["success"] = true;
    response["message"] = "OK";
    response["data"] = natureNames;
    return response.dump();
}

Nature natureFromString(const std::string& nature){
    return stringToNature.at(nature);
}

const std::string& stringFromNature(Nature nature){
    return natureToString.at(nature);
}