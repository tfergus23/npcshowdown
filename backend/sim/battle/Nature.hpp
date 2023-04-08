#pragma once
#include <string>
#include <unordered_map>

enum Nature {};

inline const std::unordered_map<std::string, const Nature> natures;

float natureBoost(Nature nature, Stat stat);