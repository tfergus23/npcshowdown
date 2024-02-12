#include "TournamentTrainer.hpp"

void TournamentTrainer::calculateElo(){
    elo = (opponentRatingsTotal + 400 * (wins - losses)) / (wins + losses);
}