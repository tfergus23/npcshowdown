#include "sim/battle/Nature.hpp"
#include "sim/battle/Stat.hpp"

float natureBoost(Nature nature, Stat stat){
    int natureIndex = (int) nature;
    int statIndex = (int) stat - 1;
    return natureChart[natureIndex][statIndex];
}