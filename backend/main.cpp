#include "api/NPCS_API_Server.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Items.hpp"
#include "sim/data/Species.hpp"

int main(){
    mapIDsToAbilities();
    mapIDsToItems();
    mapIDsToMoves();
    mapIDsToSpecies();

    
    NPCS_API_Server server;
    return server.run();
    
}