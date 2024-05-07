#include <iostream>
#include "api/NPCS_API_Server.hpp"
#include "sim/battle/Battle.hpp"
#include <chrono>
#include <fstream>



int main(){
    NPCS_API_Server server;
    return server.run();
}
