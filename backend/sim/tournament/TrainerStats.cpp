#include "TrainerStats.hpp"

void TrainerStats::calculateElo(){
    elo = (opponentRatingsTotal + 400 * (wins - losses)) / (wins + losses);
}

json TrainerStats::toJSON() const{
    json json;

    json["elo"] = elo;
    json["wins"] = wins;
    json["losses"] = losses;
    json["bestWin"] = bestWin;
    json["bestWinEloDiff"] = bestWinEloDiff;
    json["index"] = trainerIndex;

    return json;
}