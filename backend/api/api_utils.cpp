#include "api_utils.hpp"
#include <vector>
#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Abilities.hpp"
#include <iostream>
#include <climits>
#include "sim/tournament/Tournament.hpp"

const int MAX_NAME_SIZE = 64;
const std::hash<std::string> hasher;

//TODO: Use these
std::string checkForString(const json& json, const std::string& prefix, const std::string& fieldName){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_string()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be of type string, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    return problems;
}

std::string checkForInt(const json& json, const std::string& prefix, const std::string& fieldName){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_number_integer()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be of type number (integer), was " + std::string(json[fieldName].type_name()) + "\n";
    }
    return problems;
}

std::string checkForObject(const json& json, const std::string& prefix, const std::string& fieldName){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_object()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be of type object, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    return problems;
}

std::string checkForFixedIntArray(const json& json, const std::string& prefix, const std::string& fieldName, size_t arraySize){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_array()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " integers, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    else if (json[fieldName].size() != arraySize){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " integers, had " + std::to_string(json[fieldName].size()) + " elements\n";
    }
    else{
        for(int i = 0; i < json[fieldName].size(); i++){
            std::string arrayFieldName = prefix + fieldName + "[" + std::to_string(i) + "]";
            if (!json[fieldName][i].is_number_integer()){
                problems += "Bad Request: " + arrayFieldName + " must be of type number (integer), was " + std::string(json[fieldName][i].type_name()) + "\n";
            }
        }
    }
    return problems;
}

std::string checkForFixedStringArray(const json& json, const std::string& prefix, const std::string& fieldName, size_t arraySize){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_array()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " strings, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    else if (json[fieldName].size() != arraySize){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of exactly " + std::to_string(arraySize) + " strings, had " + std::to_string(json[fieldName].size()) + " elements\n";
    }
    else{
        for(int i = 0; i < json[fieldName].size(); i++){
            std::string arrayFieldName = prefix + fieldName + "[" + std::to_string(i) + "]";
            if (!json[fieldName][i].is_string()){
                problems += "Bad Request: " + arrayFieldName + " must be of type string, was " + std::string(json[fieldName][i].type_name()) + "\n";
            }
        }
    }
    return problems;
}

std::string checkForDynamicObjectArray(const json& json, const std::string& prefix, const std::string& fieldName, size_t maxArraySize){
    std::string problems = "";
    if (!json.contains(fieldName)){
        problems += "Bad Request: Undefined required field '" + prefix + fieldName + "'\n";
    }
    else if (!json[fieldName].is_array()){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of no more than " + std::to_string(maxArraySize) + " objects, was " + std::string(json[fieldName].type_name()) + "\n";
    }
    else if (json[fieldName].size() > maxArraySize){
        problems += "Bad Request: '" + prefix + fieldName + "' must be an array of no more than " + std::to_string(maxArraySize) + " objects, had " + std::to_string(json[fieldName].size()) + " elements\n";
    }
    else{
        for(int i = 0; i < json[fieldName].size(); i++){
            std::string arrayFieldName = prefix + fieldName + "[" + std::to_string(i) + "]";
            if (!json[fieldName][i].is_object()){
                problems += "Bad Request: " + arrayFieldName + " must be of type object, was " + std::string(json[fieldName][i].type_name()) + "\n";
            }
        }
    }
    return problems;
}

std::string validateBattleRequest(const json& json){
    std::string problems = "";

    //Validate schema
    problems += checkForObject(json, "", "trainer1");
    problems += checkForObject(json, "", "trainer2");
    problems += checkForString(json, "", "seed");

    if (problems != ""){
        return problems;
    }

    problems += validateTrainerJSON(json["trainer1"], "1");
    problems += validateTrainerJSON(json["trainer2"], "2");

    return problems;

}

std::string validateTrainerJSON(const json& json, const std::string& trainerNumber){
    std::string problems = "";
    std::string trainerPrefix = "trainer" + trainerNumber + ".";
    std::string trainerFriendlyName = "Trainer";
    if (trainerNumber !=  ""){
        trainerFriendlyName += " ";
    }
    trainerFriendlyName += trainerNumber;

    //Validate schema
    problems += checkForString(json, trainerPrefix, "name");
    problems += checkForString(json, trainerPrefix, "trainerLevel");
    problems += checkForDynamicObjectArray(json, trainerPrefix, "team", 6);

    if (problems != ""){
        return problems;
    }

    //Validate user errors
    if (json["name"].get<std::string>().size() > MAX_NAME_SIZE){
        problems += trainerFriendlyName + "'s name is too long, max " + std::to_string(MAX_NAME_SIZE) + " characters.\n";
    }
    else if(json["name"].get<std::string>() == ""){
        problems += trainerFriendlyName + " needs a name.\n";
    }
    std::string trainerLevel = json["trainerLevel"].get<std::string>();
    try{
        trainerLevelFromString(trainerLevel);
    }
    catch(...){
        problems += "Invalid trainer level: " + trainerLevel + "\n";
    }
    if (json["team"].size() == 0){
        problems += trainerFriendlyName + " has no pokemon.\n";
    }

    //Validate all the pokemon
    for(int i = 0; i < json["team"].size(); i++){
        problems += validatePokemonJSON(json["team"][i], trainerNumber, std::to_string(i));
    }
    return problems;
}

std::string validatePokemonJSON(const json& json,const std::string& trainerNumber, const std::string& pokemonNumber){
    std::string pokemonPrefix = "trainer" + trainerNumber  + ".team[" + pokemonNumber + "].";
    std::string pokemonFriendlyName = "Trainer " + trainerNumber + ", Pokemon " + std::to_string(stoi(pokemonNumber)+1);
    std::string problems = "";

    //Validate schema
    problems += checkForString(json, pokemonPrefix, "species");
    problems += checkForInt(json, pokemonPrefix, "level");
    problems += checkForFixedStringArray(json, pokemonPrefix, "moves", 4);
    problems += checkForString(json, pokemonPrefix, "abilityName");
    problems += checkForString(json, pokemonPrefix, "gender");
    problems += checkForFixedIntArray(json, pokemonPrefix, "ivs", 6);
    problems += checkForString(json, pokemonPrefix, "nature");
    problems += checkForString(json, pokemonPrefix, "itemName");
    problems += checkForFixedIntArray(json, pokemonPrefix, "evs", 6);
    problems += checkForString(json, pokemonPrefix, "nickname");

    if (problems != ""){
        return problems;
    }

    //Validate user errors
    try{
        speciesFromString(json["species"].get<std::string>());
    }
    catch(...){
        if (json["species"].get<std::string>() == ""){
            problems += pokemonFriendlyName + " needs a species.\n";
        }
        else{
            problems += pokemonFriendlyName + " has an invalid species: '" + json["species"].get<std::string>() + "'\n";
        }
    }
    if (json["level"].get<int>() < 1 || json["level"].get<int>() > 100){
        problems += pokemonFriendlyName + " has an invalid level.\n";
    }
    bool foundAMove = false;
    for(int i = 0; i < json["moves"].size(); i++){
        std::string friendlyName = pokemonFriendlyName + ", move " + std::to_string(i+1);
        try{
            const Move* move = moveFromString(json["moves"][i].get<std::string>());
            if (move != MOVE_NONE){
                foundAMove = true;
            }
        }
        catch (...){
            problems += friendlyName + " is invalid or unimplemented.\n";
            foundAMove = true;
        }
    }
    if (!foundAMove){
        problems += pokemonFriendlyName + " has no moves.\n";
    }
    try {
        abilityFromString(json["abilityName"].get<std::string>());
    }
    catch(...){
        problems += pokemonFriendlyName + "'s ability is invalid or unimplemented.\n";
    }
    std::string genderString = json["gender"].get<std::string>();
    if (genderString != "Random" && genderString != "Male" && genderString != "Female"){
        problems += pokemonFriendlyName + " has an invalid gender.\n";
    }

    auto ivs = json["ivs"].get<std::array<int,6>>();
    for(auto iv : ivs){
        if (iv < 0 || iv > 31){
            problems += pokemonFriendlyName + " has invalid IVs.\n";
            break;
        }
    }

    try{
        natures.at(json["nature"].get<std::string>());
    }
    catch(...){
        problems += pokemonFriendlyName + " has an invalid nature.\n";
    }

    try {
        itemFromString(json["itemName"].get<std::string>());
    }
    catch (...){
        problems += pokemonFriendlyName + "'s item is invalid or unimplemented.\n";
    }
    auto evs = json["evs"].get<std::array<int,6>>();
    for(auto ev : evs){
        if (ev < 0 || ev > 255){
            problems += pokemonFriendlyName + " has invalid EVs.\n";
            break;
        }
    }
    if (json["nickname"].get<std::string>().size() > MAX_NAME_SIZE){
        problems += pokemonFriendlyName + "'s nickname is too long, max " + std::to_string(MAX_NAME_SIZE) + " characters.\n";
    }
    return problems;
}

std::string validateTournamentRequest(const json& json){
    std::string problems = "";

    //Validate Schema
    problems += checkForDynamicObjectArray(json, "", "trainers", INT_MAX);
    problems += checkForString(json, "", "seed");
    problems += checkForInt(json, "", "rounds");

    if (problems != ""){
        return problems;
    }

    if (json["trainers"].size() < 2){
        return "Need to have at least 2 trainers to have a tournament, " + std::to_string(json["trainers"].size()) + " supplied.\n";
    }

    
    if (json["rounds"].get<int>() < 1){
        return "Can't have a tournament with zero or fewer rounds.\n";
    }
    

    if (calculateTotalBattles(json["trainers"].size(), json["rounds"].get<int>()) > MAX_TOURNAMENT_BATTLES){
        return "Too many battles in requested tournament, " + std::to_string(MAX_TOURNAMENT_BATTLES) + " max.\n";
    }

    for(size_t i = 0; i < json["trainers"].size(); i++){
        problems += validateTrainerJSON(json["trainers"][i], std::to_string(i+1));
    }

    return problems;
}

size_t seedFromString(const std::string& seedString){
    size_t seed = 0;
    try {
        seed = stoul(seedString);
    }
    catch(...){
        seed = hasher(seedString);
    }
    return seed;
}