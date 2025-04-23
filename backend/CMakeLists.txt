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

add_library(npcs_sim ${battle} ${data} ${utils} ${tournament})

add_executable (npcshowdown main.cpp ${api})

if (NPCS_BUILD_TESTS)
    include(FetchContent)
    include(CTest)
    enable_testing()
    FetchContent_Declare(
    googletest
    # Specify the commit you depend on and update it regularly.
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_executable(npcs_tests ${PROJECT_SOURCE_DIR}/tests/tests.cpp)
    
    add_test(NAME Tests COMMAND npcs_tests)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/lib/json/single_include 
    ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib/tfhttp/include 
    ${PROJECT_SOURCE_DIR}/lib/tflib/include 
    ${PROJECT_SOURCE_DIR}/lib/mariadb/include/
    
)

target_link_libraries(npcs_sim TFLib)
target_link_libraries(npcs_tests npcs_sim gtest_main)

target_link_libraries(npcshowdown 
    tfhttp 
    TFLib 
    ${PROJECT_SOURCE_DIR}/lib/mariadb/lib/mariadb/libmariadbcpp.so 
    crypto
    npcs_sim
)

