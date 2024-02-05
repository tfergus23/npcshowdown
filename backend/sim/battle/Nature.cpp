#include "sim/battle/Nature.hpp"
#include "sim/battle/Stat.hpp"
#include <vector>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

float natureBoost(Nature nature, Stat stat){
    int natureIndex = (int) nature;
    int statIndex = (int) stat - 1;
    return natureChart[natureIndex][statIndex];
}

std::string createNatureDataResponse(){
    std::vector<std::string> natureNames;
    for (auto [name,ptr] : natures){
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