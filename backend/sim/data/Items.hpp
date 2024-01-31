#pragma once
#include "sim/battle/Item.hpp"

extern const std::string ITEM_NONE_NAME;
extern const Item* ITEM_NONE;

const Item* itemFromString(const std::string& itemName);