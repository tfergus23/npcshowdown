#pragma once
#include "Stat.hpp"

// These values are used to determine the correct attacking/defending stats
enum class DamageCategory{PHYSICAL=(int)Stat::ATTACK, STATUS=0, SPECIAL=(int)Stat::SPATTACK};