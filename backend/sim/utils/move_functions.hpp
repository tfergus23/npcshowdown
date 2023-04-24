#pragma once
#include "sim/battle/MoveUse.hpp"

int dealDirectDamage(MoveUse* moveUse, bool logEffectiveness = true);

void selfDestruct(MoveUse* moveUse);
void crash(Pokemon* user, Battle* battle);