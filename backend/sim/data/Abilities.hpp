#pragma once
#include "sim/battle/Ability.hpp"

extern const Ability ABILITY_GUTS;
extern const Ability ABILITY_TORRENT;
extern const Ability ABILITY_TRUANT;

const Ability* abilityFromString(const std::string& abilityName);
const Ability* abilityFromId(int16_t id);
void mapIDsToAbilities();
std::string createAbilityDataResponse();