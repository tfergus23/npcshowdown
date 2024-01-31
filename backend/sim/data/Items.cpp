#include "sim/data/Items.hpp"
#include <unordered_map>
#include <string>

const std::string ITEM_NONE_NAME = "";
const Item* ITEM_NONE = nullptr;

const std::unordered_map<std::string, const Item*> items = {
    {ITEM_NONE_NAME, ITEM_NONE}
};

const Item* itemFromString(const std::string& itemName){
    return items.at(itemName);
}