#pragma once
#include <unordered_map>
#include <string>

enum class Gender : int8_t {MALE, FEMALE, GENDERLESS};


inline const std::unordered_map<std::string, const Gender> stringToGender = {
    {"Male", Gender::MALE},
    {"Female", Gender::FEMALE},
    {"Genderless", Gender::GENDERLESS}
};

inline const std::unordered_map<Gender, std::string> genderToString = {
    {Gender::MALE, "Male"},
    {Gender::FEMALE, "Female"},
    {Gender::GENDERLESS, "Genderless"},
};