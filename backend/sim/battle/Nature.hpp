#pragma once
#include <string>
#include <unordered_map>
#include "sim/battle/Nature.hpp"
#include "sim/battle/Stat.hpp"

enum class Nature : int8_t {HARDY,LONELY,BRAVE,ADAMANT,NAUGHTY,BOLD,DOCILE,RELAXED,IMPISH,LAX,TIMID,HASTY,SERIOUS,JOLLY,NAIVE,MODEST,MILD,QUIET,BASHFUL,RASH,CALM,GENTLE,SASSY,CAREFUL,QUIRKY};

inline const std::unordered_map<std::string, Nature> natures = {
    {"Hardy", Nature::HARDY},
    {"Lonely", Nature::LONELY},
    {"Brave", Nature::BRAVE},
    {"Adamant", Nature::ADAMANT},
    {"Naughty", Nature::NAUGHTY},
    {"Bold", Nature::BOLD},
    {"Docile", Nature::DOCILE},
    {"Relaxed", Nature::RELAXED},
    {"Impish", Nature::IMPISH},
    {"Lax", Nature::LAX},
    {"Timid", Nature::TIMID},
    {"Hasty", Nature::HASTY},
    {"Serious", Nature::SERIOUS},
    {"Jolly", Nature::JOLLY},
    {"Modest", Nature::MODEST},
    {"Mild", Nature::MILD},
    {"Quiet", Nature::QUIET},
    {"Bashful", Nature::BASHFUL},
    {"Rash", Nature::RASH},
    {"Calm", Nature::CALM},
    {"Gentle", Nature::GENTLE},
    {"Sassy", Nature::SASSY},
    {"Careful", Nature::CAREFUL},
    {"Quirky", Nature::QUIRKY}
};

inline const float natureChart[25][5] = {
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.1f,0.9f,1.0f,1.0f,1.0f},
    {1.1f,1.0f,1.0f,1.0f,0.9f},
    {1.1f,1.0f,0.9f,1.0f,1.0f},
    {1.1f,1.0f,1.0f,0.9f,1.0f},
    {0.9f,1.1f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.1f,1.0f,1.0f,0.9f},
    {1.0f,1.1f,0.9f,1.0f,1.0f},
    {1.0f,1.1f,1.0f,0.9f,1.0f},
    {0.9f,1.0f,1.0f,1.0f,1.1f},
    {1.0f,0.9f,1.0f,1.0f,1.1f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,0.9f,1.0f,1.1f},
    {1.0f,1.0f,1.0f,0.9f,1.1f},
    {0.9f,1.0f,1.1f,1.0f,1.0f},
    {1.0f,0.9f,1.1f,1.0f,1.0f},
    {1.0f,1.0f,1.1f,1.0f,0.9f},
    {1.0f,1.0f,1.0f,1.0f,1.0f},
    {1.0f,1.0f,1.1f,0.9f,1.0f},
    {0.9f,1.0f,1.0f,1.1f,1.0f},
    {1.0f,0.9f,1.0f,1.1f,1.0f},
    {1.0f,1.0f,1.0f,1.1f,0.9f},
    {1.0f,1.0f,0.9f,1.1f,1.0f},
    {1.0f,1.0f,1.0f,1.0f,1.0f}
};

float natureBoost(Nature nature, Stat stat);
std::string createNatureDataResponse();