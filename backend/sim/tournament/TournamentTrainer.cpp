#include "TournamentTrainer.hpp"

void TournamentTrainer::calculateElo(){
    elo = (opponentRatingsTotal + 400 * (wins - losses)) / (wins + losses);
}
/*
    const Trainer& trainer;
    int elo = 1000;
    int wins = 0;
    int losses = 0;
    int opponentRatingsTotal = 0;
    //Index into the results list
    int bestWin = -1;
    int bestWinEloDiff = INT_MIN;
*/

json TournamentTrainer::toJSON() const{
    json json;

    json["elo"] = elo;
    json["wins"] = wins;
    json["losses"] = losses;
    json["bestWin"] = bestWin;
    json["bestWinEloDiff"] = bestWinEloDiff;
    json["index"] = trainerIndex;

    return json;
}