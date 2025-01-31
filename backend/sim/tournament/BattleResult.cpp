#include "./BattleResult.hpp"

json BattleResult::toJSON() const{
    json json;

    json["trainer1Index"] = trainer1;
    json["trainer2Index"] = trainer2;
    json["seed"] = seed;

    return json;
}