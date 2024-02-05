#pragma once
#include "sim/battle/Ability.hpp"

extern const std::string ABILITY_NONE_NAME;
extern const Ability* ABILITY_NONE;
extern const Ability ABILITY_GUTS;
extern const Ability ABILITY_TORRENT;

const Ability* abilityFromString(const std::string& abilityName);
std::string createAbilityDataResponse();