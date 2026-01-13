#pragma once
#include "sim/battle/Item.hpp"

extern const Item ITEM_NONE;
extern const Item ITEM_LEFTOVERS;
extern const Item ITEM_BLACK_SLUDGE;

const Item* itemFromString(const std::string& itemName);
void mapIDsToItems();
const Item* itemFromID(int16_t id);
std::string createItemDataResponse();