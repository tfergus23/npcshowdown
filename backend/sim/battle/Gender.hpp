#pragma once
#include <unordered_map>
#include <string>

enum Gender{MALE, FEMALE, GENDERLESS};


inline std::unordered_map<std::string, const Gender> genders = {
    {"Male", MALE},
    {"Female", FEMALE},
    {"Genderless", GENDERLESS}
};