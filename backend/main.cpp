#include "api/NPCS_API_Server.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Items.hpp"
#include "sim/data/Species.hpp"
#include <fcntl.h>
#include <unistd.h>

bool already_running(const char* lock_file_path) {
    int fd = open(lock_file_path, O_CREAT | O_RDWR, 0666);
    if (fd < 0) return true;

    if (lockf(fd, F_TLOCK, 0) < 0) {
        close(fd);
        return true;
    }

    dprintf(fd, "%d\n", getpid());

    return false;
}

int main(){
    if (already_running("/tmp/.npcs.lock")){
        std::cerr << "Another instance of NPCShowdown is already running.\n";
        return 1;
    }
#ifndef NDEBUG
    std::cout.setf(std::ios::unitbuf);
#endif
    std::cout << "Creating ID mappings... ";
    mapIDsToAbilities();
    mapIDsToItems();
    mapIDsToMoves();
    mapIDsToSpecies();
    std::cout << "Done.\n";

    
    NPCS_API_Server server;
    return server.run();
    
}