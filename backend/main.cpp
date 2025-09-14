#include "api/NPCS_API_Server.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Items.hpp"
#include "sim/data/Species.hpp"
#ifdef __linux__
#include <unistd.h>
#include <fcntl.h>
#endif
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#endif
#include "tflib/strings.h"
#include "tflib/arg_parser.h"

#ifdef _WIN32
bool already_running(const char* lock_file_path) {
    HANDLE hFile = CreateFileA(
        lock_file_path,
        GENERIC_READ | GENERIC_WRITE,
        0, // No sharing = exclusive access
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return true; // Could not open the file
    }

    OVERLAPPED overlapped = {0};
    if (!LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapped)) {
        CloseHandle(hFile);
        return true; // Failed to acquire lock
    }

    // Write PID to file
    DWORD pid = GetCurrentProcessId();
    char pid_str[32];
    int len = snprintf(pid_str, sizeof(pid_str), "%lu\n", pid);
    DWORD written;
    WriteFile(hFile, pid_str, len, &written, NULL);

    // Do NOT close the file — closing releases the lock
    // Keep it open for the life of the process
    return false;
}
#endif

#ifdef __linux__
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
#endif

inline void generateTypeAssertions(){
    for (uint8_t i = 0; i < 19; i++){
        for (uint8_t j = 0; j < 19; j++){
            for (uint8_t k = j+1; k < 19; k++){
                std::string attack = tflib::to_upper(typeString((Type) i));
                std::string defend1 = tflib::to_upper(typeString((Type) j));
                std::string defend2 = tflib::to_upper(typeString((Type) k));
                float matchup = typeMatchup((Type) i, (Type) j, (Type) k);
                std::string matchupString = "1.0f";
                if (matchup == ULTRA_EFFECTIVE){
                    matchupString = "ULTRA_EFFECTIVE";
                }
                else if (matchup == SUPER_EFFECTIVE){
                    matchupString = "SUPER_EFFECTIVE";
                }
                else if (matchup == NOT_VERY_EFFECTIVE){
                    matchupString = "NOT_VERY_EFFECTIVE";
                }
                else if (matchup == BARELY_EFFECTIVE){
                    matchupString = "BARELY_EFFECTIVE";
                }
                else if (matchup == NOT_EFFECTIVE){
                    matchupString = "NOT_EFFECTIVE";
                }
                std::cout << "static_assert(typeMatchup(Type::" + attack + ", Type::" + defend1 + ", Type::" + defend2 + ") == " + matchupString + ");\n";
            }
        }
    }
}

int main(int argc, char** argv){
#ifdef __linux__
    const char* lock_file = "/tmp/.npcs.lock";
#endif
#ifdef _WIN32
    const char* lock_file = "./.npcs.lock";
#endif
    if (already_running(lock_file)){
        std::cerr << "Another instance of NPCShowdown is already running.\n";
        return 1;
    }
#ifndef NDEBUG
    std::cout.setf(std::ios::unitbuf);
#endif
    tflib::arg_parser args;
    args.add_flag("test", 't');
    args.parse(argc, argv);

    std::cout << "Creating ID mappings... ";
    mapIDsToAbilities();
    mapIDsToItems();
    mapIDsToMoves();
    mapIDsToSpecies();
    std::cout << "Done.\n";

    // Just for making sure all globals initialize correctly and nothing goes wrong with mapping IDs
    if (args.get_flag("test")){
        return 0;
    }
    
    NPCS_API_Server server;
    return server.run();
    
}