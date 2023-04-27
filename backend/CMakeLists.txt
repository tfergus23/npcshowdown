# CMakeList.txt : CMake project for NPCShowdown, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("NPCShowdown")

# Add source to this project's executable.
FILE(GLOB battle ${PROJECT_SOURCE_DIR}/sim/battle/*.cpp)
FILE(GLOB data ${PROJECT_SOURCE_DIR}/sim/data/*.cpp)
FILE(GLOB tournament ${PROJECT_SOURCE_DIR}/sim/tournament/*.cpp)
FILE(GLOB utils ${PROJECT_SOURCE_DIR}/sim/utils/*.cpp)

add_executable (npcs_sim main.cpp ${battle} ${data} ${tournament} ${utils})
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib/json/single_include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET npcs_sim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
