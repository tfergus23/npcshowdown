#include "nlohmann/json.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/tournament/BattleResult.hpp"
#include "sim/tournament/Tournament.hpp"

using json = nlohmann::json;

struct TournamentResults{
    std::vector<TrainerStats> trainerStats;
    std::vector<size_t> trainers;
    bool ready = false;
};

namespace db{
    json getTrainer(size_t id);
    BattleResult getBattle(size_t id);
    TournamentResults getTournament(size_t id);

    size_t createEmptyTournament();
    size_t saveTrainer(const json& json);
    size_t saveBattle(const Trainer& trainer1, const Trainer& trainer2, size_t seed);
    size_t saveBattle(const BattleResult result);
    void saveTournament(const Tournament& tournament, size_t id);
};
