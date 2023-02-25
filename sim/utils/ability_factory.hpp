#pragma once

#include "sim/battle/Ability.hpp"
#include <unordered_map>
#include <string>
#include <vector>

Ability createAbility();
void setAbilityNames();

std::vector<std::string> abilityNames;