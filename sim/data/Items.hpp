#pragma once
#include "battle/Item.hpp"
#include <unordered_map>
#include <string>

inline const std::string ITEM_NONE_NAME = "";
inline const Item* ITEM_NONE = nullptr;

std::unordered_map<std::string, const Item*> items = {
    {ITEM_NONE_NAME, ITEM_NONE}
};