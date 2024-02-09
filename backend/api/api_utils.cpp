#include "api_utils.hpp"
#include <vector>
#include "sim/battle/Battle.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Abilities.hpp"
#include <iostream>

const int MAX_NAME_SIZE = 32;


std::string validateBattleRequest(const json& json){
    std::string problems = "";

    if (!json.contains("trainer1")){
        problems += "Bad Request: Undefined required field 'trainer1'\n";
    }
    if (!json.contains("trainer2")){
        problems += "Bad Request: Undefined required field 'trainer2'\n";
    }
    if (!json.contains("seed")){
        problems += "Bad Request: Undefined required field 'seed'\n";
    }
    if (problems != ""){
        return problems;
    }
    if (!json["trainer1"].is_object()){
        problems += "Bad Request: 'trainer1' must be an object, was " + std::string(json["trainer1"].type_name()) + "\n";
    }
    if (!json["trainer2"].is_object()){
        problems += "Bad Request: 'trainer2' must be an object, was " + std::string(json["trainer2"].type_name()) + "\n";
    }
    if (!json["seed"].is_string()){
        problems += "Bad Request: 'seed' must be of type string, was " + std::string(json["seed"].type_name()) + "\n";
    }
    if (problems != ""){
        return problems;
    }

    problems += validateTrainerJSON(json["trainer1"], "1");
    problems += validateTrainerJSON(json["trainer2"], "2");
    return problems;

}

std::string validateTrainerJSON(const json& json, const std::string& trainerNumber){
    std::string problems = "";
    std::string trainerFieldName = "trainer" + trainerNumber;
    std::string trainerFriendlyName = "Trainer";
    if (trainerNumber !=  ""){
        trainerFriendlyName += " ";
    }
    trainerFriendlyName += trainerNumber;

    //Validate fields defined
    if (!json.contains("name")){
        problems += "Bad Request: Undefined required field '" + trainerFieldName + ".name'\n";
    }
    if (!json.contains("trainerClass")){
        problems += "Bad Request: Undefined required field '" + trainerFieldName + ".trainerClass'\n";
    }
    if (!json.contains("trainerLevel")){
        problems += "Bad Request: Undefined required field '" + trainerFieldName + ".trainerLevel'\n";
    }
    if (!json.contains("team")){
        problems += "Bad Request: Undefined required field '" + trainerFieldName + ".team'\n";
    }
    if (problems != ""){
        return problems;
    }

    //Validate correct types
    if (!json["name"].is_string()){
        problems += "Bad Request: " + trainerFieldName + ".name must be of type string, was " + std::string(json["name"].type_name()) + "\n";
    }
    if (!json["trainerClass"].is_string()){
        problems += "Bad Request: " + trainerFieldName + ".trainerClass must be of type string, was " + std::string(json["trainerClass"].type_name()) + "\n";
    }
    if (!json["trainerLevel"].is_string()){
        problems += "Bad Request: " + trainerFieldName + ".trainerLevel must be of type string, was " + std::string(json["trainerLevel"].type_name()) + "\n";
    }
    if (!json["team"].is_array()){
        problems += "Bad Request: " + trainerFieldName + ".team must be an array with no more than 6 elements, was " + std::string(json["team"].type_name()) + "\n";
    }
    else if (json["team"].size() > 6){
        problems += "Bad Request: " + trainerFieldName + ".team must be an array with no more than 6 elements, had " + std::to_string(json["team"].size()) + "\n";
    }
    if (problems != ""){
        return problems;
    }

    //Validate user errors
    if (json["name"].get<std::string>().size() > MAX_NAME_SIZE){
        problems += trainerFriendlyName + "'s name is too long, max " + std::to_string(MAX_NAME_SIZE) + " characters.\n";
    }
    if (json["trainerClass"].get<std::string>().size() > MAX_NAME_SIZE){
        problems += trainerFriendlyName + "'s trainer class is too long, max " + std::to_string(MAX_NAME_SIZE) + " characters.\n";
    }
    std::string trainerLevel = json["trainerLevel"].get<std::string>();
    try{
        Trainer::levelFromString(trainerLevel);
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
    std::string pokemonFieldName = "trainer" + trainerNumber  + ".team[" + pokemonNumber + "]";
    std::string pokemonFriendlyName = "Trainer " + trainerNumber + ", Pokemon " + std::to_string(stoi(pokemonNumber)+1);
    std::string problems = "";
    //Validate fields defined
    if (!json.contains("species")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".species'\n";
    }
    if (!json.contains("level")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".level'\n";
    }
    if (!json.contains("moves")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".moves'\n";
    }
    if (!json.contains("abilityName")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".abilityName'\n";
    }
    if (!json.contains("gender")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".gender'\n";
    }
    if (!json.contains("ivs")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".ivs'\n";
    }
    if (!json.contains("nature")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".nature'\n";
    }
    if (!json.contains("itemName")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".itemName'\n";
    }
    if (!json.contains("evs")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".evs'\n";
    }
    if (!json.contains("nickname")){
        problems += "Bad Request: Undefined required field '" + pokemonFieldName + ".nickname'\n";
    }

    if (problems != ""){
        return problems;
    }

    //Validate correct types
    if (!json["species"].is_string()){
        problems += "Bad Request: " + pokemonFieldName + ".species must be of type string, was "
            + std::string(json["species"].type_name()) + "\n";
    }
    if (!json["level"].is_number_integer()){
        problems += "Bad Request: " + pokemonFieldName + ".level must be of type number (integer), was "
            + std::string(json["level"].type_name()) + "\n";
    }
    if (!json["moves"].is_array()){
        problems += "Bad Request: " + pokemonFieldName + ".moves must be an array of exactly 4 strings, was "
            + std::string(json["moves"].type_name()) + "\n";
    }
    else if (json["moves"].size() != 4){
        problems += "Bad Request: " + pokemonFieldName + ".moves must be an array of exactly 4 strings, had "
            + std::to_string(json["moves"].size()) + "\n";
    }
    else{
        for(int i = 0; i < json["moves"].size(); i++){
            std::string fieldName = pokemonFieldName + ".moves[" + std::to_string(i) +  "]";
            if (!json["moves"][i].is_string()){
                problems += "Bad Request: " + fieldName + " must be of type string, was " + std::string(json["moves"][i].type_name()) + "\n";
            }
        }
    }
    if (!json["abilityName"].is_string()){
        problems += "Bad Request: " + pokemonFieldName + ".abilityName must be of type string, was "
            + std::string(json["abilityName"].type_name()) + "\n";
    }
    if (!json["gender"].is_string()){
        problems += "Bad Request: " + pokemonFieldName + ".gender must be of type string, was "
            + std::string(json["gender"].type_name()) + "\n";
    }
    if (!json["ivs"].is_array()){
        problems += "Bad Request: " + pokemonFieldName + ".ivs must be an array of exactly 6 integers, was "
            + std::string(json["ivs"].type_name()) + "\n";
    }
    else if (json["ivs"].size() != 6){
        problems += "Bad Request: " + pokemonFieldName + ".ivs must be an array of exactly 6 integers, had "
            + std::to_string(json["ivs"].size()) + "\n";
    }
    else{
        for(int i = 0; i < json["ivs"].size(); i++){
            std::string fieldName = pokemonFieldName + ".ivs[" + std::to_string(i) +  "]";
            if (!json["ivs"][i].is_number_integer()){
                problems += "Bad Request: " + fieldName + " must be of type number (integer), was " + std::string(json["ivs"][i].type_name()) + "\n";
            }
        }
    }
    if (!json["nature"].is_string()){
        problems += "Bad Request: " + pokemonFieldName + ".nature must be of type string, was "
            + std::string(json["nature"].type_name()) + "\n";
    }
    if (!json["itemName"].is_string()){
        problems += "Bad Request: " + pokemonFieldName + ".itemName must be of type string, was "
            + std::string(json["itemName"].type_name()) + " (use empty string for no item)" + "\n";
    }
    if (!json["evs"].is_array()){
        problems += "Bad Request: " + pokemonFieldName + ".evs must be an array of exactly 6 integers, was "
            + std::string(json["evs"].type_name()) + "\n";
    }
    else if (json["evs"].size() != 6){
        problems += "Bad Request: " + pokemonFieldName + ".evs must be an array of exactly 6 integers, had "
            + std::to_string(json["evs"].size()) + "\n";
    }
    else{
        for(int i = 0; i < json["evs"].size(); i++){
            std::string fieldName = pokemonFieldName + ".evs[" + std::to_string(i) +  "]";
            if (!json["evs"][i].is_number_integer()){
                problems += "Bad Request: " + fieldName + " must be of type number (integer), was " + std::string(json["evs"][i].type_name()) + "\n";
            }
        }
    }
    if (!json["nickname"].is_string()){
        problems += "Bad Request: " + pokemonFieldName + ".nickname must be of type string, was "
            + std::string(json["nickname"].type_name()) + "\n";
    }

    if (problems != ""){
        return problems;
    }

    //Validate user errors
    //TODO: Remove these try catches
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