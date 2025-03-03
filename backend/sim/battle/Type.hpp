#pragma once
#include <stdint.h>

enum class Type : uint8_t {NONE,NORMAL,FIRE,WATER,ELECTRIC,GRASS,ICE,FIGHTING,POISON,GROUND,FLYING,PSYCHIC,BUG,ROCK,GHOST,DRAGON,DARK,STEEL,FAIRY};

inline constexpr float typeChart[19][19] = {
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.5f,0.0f,1.0f,1.0f,0.5f,1.0f},
    {1.0f,1.0f,0.5f,0.5f,1.0f,2.0f,2.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,0.5f,1.0f,2.0f,1.0f},
    {1.0f,1.0f,2.0f,0.5f,1.0f,0.5f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,2.0f,0.5f,0.5f,1.0f,1.0f,1.0f,0.0f,2.0f,1.0f,1.0f,1.0f,1.0f,0.5f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,0.5f,2.0f,1.0f,0.5f,1.0f,1.0f,0.5f,2.0f,0.5f,1.0f,0.5f,2.0f,1.0f,0.5f,1.0f,0.5f,1.0f},
    {1.0f,1.0f,0.5f,0.5f,1.0f,2.0f,0.5f,1.0f,1.0f,2.0f,2.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f},
    {1.0f,2.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,0.5f,0.5f,0.5f,2.0f,0.0f,1.0f,2.0f,2.0f,0.5f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,0.5f,0.5f,1.0f,1.0f,1.0f,0.5f,0.5f,1.0f,1.0f,0.0f,2.0f},
    {1.0f,1.0f,2.0f,1.0f,2.0f,0.5f,1.0f,1.0f,2.0f,1.0f,0.0f,1.0f,0.5f,2.0f,1.0f,1.0f,1.0f,2.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,0.5f,2.0f,1.0f,2.0f,1.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,1.0f,1.0f,0.5f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,1.0f,1.0f,0.5f,1.0f,1.0f,1.0f,1.0f,0.0f,0.5f,1.0f},
    {1.0f,1.0f,0.5f,1.0f,1.0f,2.0f,1.0f,0.5f,0.5f,1.0f,0.5f,2.0f,1.0f,1.0f,0.5f,1.0f,2.0f,0.5f,0.5f},
    {1.0f,1.0f,2.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,0.5f,2.0f,1.0f,2.0f,1.0f,1.0f,1.0f,1.0f,0.5f,1.0f},
    {1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,0.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.5f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,0.5f},
    {1.0f,1.0f,0.5f,0.5f,0.5f,1.0f,2.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,1.0f,0.5f,2.0f},
    {1.0f,1.0f,0.5f,1.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,0.5f,1.0f}
};

constexpr float typeMatchup(Type attackingType, Type defendingType1, Type defendingType2){
    int attackingIndex = (int) attackingType;
    int defendingIndex1 = (int) defendingType1;
    int defendingIndex2 = (int) defendingType2;
    return typeChart[attackingIndex][defendingIndex1] * typeChart[attackingIndex][defendingIndex2];
}

constexpr float ULTRA_EFFECTIVE = 4.0f;
constexpr float SUPER_EFFECTIVE = 2.0f;
constexpr float NORMAL_EFFECTIVE = 1.0f;
constexpr float NOT_VERY_EFFECTIVE = 0.5f;
constexpr float BARELY_EFFECTIVE = 0.25f;
constexpr float NOT_EFFECTIVE = 0.0f;