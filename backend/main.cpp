#include "api/NPCS_API_Server.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Items.hpp"
#include "sim/data/Species.hpp"

int main(){
#ifndef NDEBUG
    std::cout.setf(std::ios::unitbuf);
#endif
    mapIDsToAbilities();
    mapIDsToItems();
    mapIDsToMoves();
    mapIDsToSpecies();

    
    NPCS_API_Server server;
    return server.run();
    
}