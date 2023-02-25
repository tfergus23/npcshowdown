# CMakeList.txt : CMake project for NPCShowdown, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("NPCShowdown")

# Add source to this project's executable.
add_executable (npcs_sim main.cpp)
include_directories(${PROJECT_SOURCE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET npcs_sim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
