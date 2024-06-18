#pragma once
#include <unordered_map>
#include <string>

enum class Gender{MALE, FEMALE, GENDERLESS};


inline const std::unordered_map<std::string, const Gender> genders = {
    {"Male", Gender::MALE},
    {"Female", Gender::FEMALE},
    {"Genderless", Gender::GENDERLESS}
};