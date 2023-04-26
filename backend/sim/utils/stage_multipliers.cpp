#include "sim/utils/stage_multipliers.hpp"

float statStageMultiplier(int stage){
    int index = stage + 6;
    return statMultipliers[index];
}
float accuracyStageMultiplier(int accuracyStage){
    int index = accuracyStage + 6;
    return accuracyMultipliers[index];
}
float critChanceFromStage(int stage){
    if (stage < 4) return critChances[stage] * 100.0f;
    return 50.0f;
}