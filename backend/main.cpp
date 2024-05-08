#include "api/NPCS_API_Server.hpp"

const size_t MAX_REQUEST_SIZE = 524288;

int main(){
    NPCS_API_Server server;
    return server.run();
}