#pragma once
#include <string>
#include <unordered_map>
#include "sim/battle/Nature.hpp"
#include "sim/battle/Stat.hpp"

enum class Nature : int8_t {HARDY,LONELY,BRAVE,ADAMANT,NAUGHTY,BOLD,DOCILE,RELAXED,IMPISH,LAX,TIMID,HASTY,SERIOUS,JOLLY,NAIVE,MODEST,MILD,QUIET,BASHFUL,RASH,CALM,GENTLE,SASSY,CAREFUL,QUIRKY};

float natureBoost(Nature nature, Stat stat);
Nature natureFromString(const std::string& nature);
const std::string& stringFromNature(Nature nature);
std::string createNatureDataResponse();