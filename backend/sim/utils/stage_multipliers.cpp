#include "sim/utils/stage_multipliers.hpp"

static constexpr float statMultipliers[] = { 2.0f / 8.0f, 2.0f / 7.0f, 2.0f / 6.0f, 2.0f / 5.0f, 2.0f / 4.0f, 2.0f / 3.0f, 2.0f / 2.0f, 3.0f / 2.0f, 4.0f / 2.0f, 5.0f / 2.0f, 6.0f / 2.0f, 7.0f / 2.0f, 8.0f / 2.0f };
static constexpr float accuracyMultipliers[] = { 33.0f/ 100.0f, 36.0f/ 100.0f, 43.0f/ 100.0f, 50.0f/ 100.0f, 60.0f/ 100.0f, 75.0f/ 100.0f, 100.0f/ 100.0f, 133.0f/ 100.0f, 166.0f/ 100.0f, 200.0f/ 100.0f, 250.0f/ 100.0f, 266.0f/ 100.0f, 300.0f/ 100.0f};
static constexpr float critChances[] = { 1.0f / 16.0f, 1.0f / 8.0f, 1.0f / 4.0f, 1.0f / 3.0f};

constexpr float statStageMultiplier(int stage){
    int index = stage + 6;
    return statMultipliers[index];
}
constexpr float accuracyStageMultiplier(int accuracyStage){
    int index = accuracyStage + 6;
    return accuracyMultipliers[index];
}
constexpr float critChance(int stage){
    if (stage < 4) return critChances[stage] * 100.0f;
    return 50.0f;
}