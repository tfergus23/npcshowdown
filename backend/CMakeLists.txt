# CMakeList.txt : CMake project for NPCShowdown, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.22)

project ("NPCShowdown")
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/expresscpp)
# Add source to this project's executable.
FILE(GLOB battle ${PROJECT_SOURCE_DIR}/sim/battle/*.cpp)
FILE(GLOB data ${PROJECT_SOURCE_DIR}/sim/data/*.cpp)
FILE(GLOB tournament ${PROJECT_SOURCE_DIR}/sim/tournament/*.cpp)
FILE(GLOB utils ${PROJECT_SOURCE_DIR}/sim/utils/*.cpp)
FILE(GLOB api ${PROJECT_SOURCE_DIR}/api/*.cpp)

add_executable (npcs_sim main.cpp ${battle} ${data} ${tournament} ${utils} ${api})
include_directories(${PROJECT_SOURCE_DIR}/lib/expresscpp/include ${PROJECT_SOURCE_DIR}/lib/json/single_include ${PROJECT_SOURCE_DIR})

target_link_libraries(npcs_sim PRIVATE expresscpp::expresscpp)