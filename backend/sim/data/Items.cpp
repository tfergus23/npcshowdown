#include "sim/data/Items.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "sim/utils/MoveFunctions.hpp"
using json = nlohmann::json;

const Item ITEM_NONE = {
    .name = "",
    .id = 0
};

const Item ITEM_LEFTOVERS = {
    .observer = {
        .endOfTurn = [](Pokemon* subject, Battle* battle, const EventArgs& args){
            //Heal Block doesn't allow you to heal... figure that out later
            MoveFunctions::givePercentHealing(6.25f, subject, battle);
        }
    },
    .name = "Leftovers",
    .id = 1,
};

const Item ITEM_BLACK_SLUDGE;

const std::unordered_map<std::string, const Item*> items = {
    {ITEM_NONE.name, &ITEM_NONE},
    {ITEM_LEFTOVERS.name, &ITEM_LEFTOVERS}
};

static std::unordered_map<int16_t,const Item*> idToItemMap;

void mapIDToItem(int16_t id, const Item* item){
    assert(!idToItemMap.contains(id));
    assert(id == item->id);
    idToItemMap[id] = item;
}

void mapIDsToItems(){
    assert(items.size() > 0);
    for (const auto& [str, item] : items){
        mapIDToItem(item->id, item);
    }
}
const Item* itemFromID(int16_t id){
    return idToItemMap.at(id);
}

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