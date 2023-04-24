#pragma once

enum Type{NORMAL,FIRE,WATER,ELECTRIC,GRASS,ICE,FIGHTING,POISON,GROUND,FLYING,PSYCHIC,BUG,ROCK,GHOST,DRAGON,DARK,STEEL,NONE,FAIRY};

float typeMatchup(Type attackingType, Type defendingType1, Type defendingType2);

float typeChart[19][19] = {
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.5f,0.0f,1.0f,1.0f,0.5f,1.0f,1.0f},
    {1.0f,0.5f,0.5f,1.0f,2.0f,2.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,0.5f,1.0f,2.0f,1.0f,1.0f},
    {1.0f,2.0f,0.5f,1.0f,0.5f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,2.0f,0.5f,0.5f,1.0f,1.0f,1.0f,0.0f,2.0f,1.0f,1.0f,1.0f,1.0f,0.5f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,0.5f,2.0f,1.0f,0.5f,1.0f,1.0f,0.5f,2.0f,0.5f,1.0f,0.5f,2.0f,1.0f,0.5f,1.0f,0.5f,1.0f,1.0f},
    {1.0f,0.5f,0.5f,1.0f,2.0f,0.5f,1.0f,1.0f,2.0f,2.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,1.0f},
    {2.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,0.5f,0.5f,0.5f,2.0f,0.0f,1.0f,2.0f,2.0f,1.0f,0.5f},
    {1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,0.5f,0.5f,1.0f,1.0f,1.0f,0.5f,0.5f,1.0f,1.0f,0.0f,1.0f,2.0f},
    {1.0f,2.0f,1.0f,2.0f,0.5f,1.0f,1.0f,2.0f,1.0f,0.0f,1.0f,0.5f,2.0f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,0.5f,2.0f,1.0f,2.0f,1.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,1.0f,1.0f,0.5f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,1.0f,1.0f,0.5f,1.0f,1.0f,1.0f,1.0f,0.0f,0.5f,1.0f,1.0f},
    {1.0f,0.5f,1.0f,1.0f,2.0f,1.0f,0.5f,0.5f,1.0f,0.5f,2.0f,1.0f,1.0f,0.5f,1.0f,2.0f,0.5f,1.0f,0.5f},
    {1.0f,2.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,0.5f,2.0f,1.0f,2.0f,1.0f,1.0f,1.0f,1.0f,0.5f,1.0f,1.0f},
    {0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,2.0f,1.0f,0.5f,0.5f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,0.5f,1.0f,0.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.5f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,2.0f,1.0f,0.5f,0.5f,1.0f,0.5f},
    {1.0f,0.5f,0.5f,0.5f,1.0f,2.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,1.0f,1.0f,1.0f,0.5f,1.0f,2.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,0.5f,1.0f,1.0f,1.0f,1.0f,2.0f,0.5f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,0.5f,1.0f,1.0f}
};

const float ULTRA_EFFECTIVE = 4.0f;
const float SUPER_EFFECTIVE = 2.0f;
const float NORMAL_EFFECTIVE = 1.0f;
const float NOT_VERY_EFFECTIVE = 0.5f;
const float BARELY_EFFECTIVE = 0.25f;
const float NOT_EFFECTIVE = 0.0f;