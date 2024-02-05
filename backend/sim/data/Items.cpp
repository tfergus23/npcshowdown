#include "sim/data/Items.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "sim/utils/move_functions.hpp"
using json = nlohmann::json;

const std::string ITEM_NONE_NAME = "";
const Item* ITEM_NONE = nullptr;

const Item ITEM_LEFTOVERS = {
    .observer = {
        .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& args){
            //Heal Block doesn't allow you to heal... figure that out later
            givePercentHealing(6.25f, subject, battle);
        }
    },
    .name = "Leftovers",
};

const std::unordered_map<std::string, const Item*> items = {
    {ITEM_NONE_NAME, ITEM_NONE},
    {ITEM_LEFTOVERS.name, &ITEM_LEFTOVERS}
};

const Item* itemFromString(const std::string& itemName){
    return items.at(itemName);
}

std::string createItemDataResponse(){
    std::vector<std::string> itemNames;
    for (auto [name,ptr] : items){
        itemNames.push_back(name);
    }
    std::sort(itemNames.begin(), itemNames.end());
    json response;
    response["success"] = true;
    response["message"] = "OK";
    response["data"] = itemNames;
    return response.dump();
}