#include "sim/battle/Type.hpp"

float typeMatchup(Type attackingType, Type defendingType1, Type defendingType2){
    int attackingIndex = (int) attackingType;
    int defendingIndex1 = int(defendingType1);
    int defendingIndex2 = int(defendingType2);
    return typeChart[attackingIndex][defendingIndex1] * typeChart[attackingIndex][defendingIndex2];
}