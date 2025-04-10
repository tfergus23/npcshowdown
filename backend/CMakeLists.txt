# CMakeList.txt : CMake project for NPCShowdown, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.22)

project ("NPCShowdown")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/tfhttp)

# Add source to this project's executable.
FILE(GLOB battle ${PROJECT_SOURCE_DIR}/sim/battle/*.cpp)
FILE(GLOB data ${PROJECT_SOURCE_DIR}/sim/data/*.cpp)
FILE(GLOB tournament ${PROJECT_SOURCE_DIR}/sim/tournament/*.cpp)
FILE(GLOB utils ${PROJECT_SOURCE_DIR}/sim/utils/*.cpp)
FILE(GLOB api ${PROJECT_SOURCE_DIR}/api/*.cpp)



add_executable (npcs_sim main.cpp ${battle} ${data} ${tournament} ${utils} ${api})

if (NPCS_BUILD_TESTS)
    add_executable(test_damage_calc ${PROJECT_SOURCE_DIR}/tests/DamageCalc.cpp ${battle} ${data} ${utils})
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/lib/json/single_include 
    ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib/tfhttp/include 
    ${PROJECT_SOURCE_DIR}/lib/tflib/include 
    ${PROJECT_SOURCE_DIR}/lib/mariadb/include/
)

target_link_libraries(npcs_sim 
    tfhttp 
    TFLib 
    ${PROJECT_SOURCE_DIR}/lib/mariadb/lib/mariadb/libmariadbcpp.so 
    crypto
)

