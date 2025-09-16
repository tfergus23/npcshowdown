# CMakeList.txt : CMake project for NPCShowdown, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.22)

project ("NPCShowdown")

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/tfhttp)

# Add source to this project's executable.
FILE(GLOB_RECURSE sim ${PROJECT_SOURCE_DIR}/sim/*.cpp)
FILE(GLOB api ${PROJECT_SOURCE_DIR}/api/*.cpp)

add_library(npcs_sim ${sim})

add_executable (npcshowdown main.cpp ${api})

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(npcs_sim PRIVATE -Werror -Wreturn-type)
	target_compile_options(npcshowdown PRIVATE -Werror -Wreturn-type)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(npcs_sim PRIVATE /WX)
	target_compile_options(npcshowdown PRIVATE /WX)
endif()

# Fetch the MariaDB C Connector (required dependency)
FetchContent_Declare(
  mariadb_c
  GIT_REPOSITORY git@github.com:mariadb-corporation/mariadb-connector-c.git
  GIT_TAG        v3.4.5
)

# Dummy install dir so mariadbc doesn't get mad
set(INSTALL_PLUGINDIR {CMAKE_BINARY_DIR}/lib)

set(WITH_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(MARIADB_LINK_DYNAMIC OFF CACHE BOOL "" FORCE)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # mariadb-c doesn't set this if its a subproject for some reason, even though they are needed for it to build.
	ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN -DNOGDI)
endif()

FetchContent_MakeAvailable(mariadb_c)


# Fetch the MariaDB C++ Connector
FetchContent_Declare(
  mariadb_cpp
  GIT_REPOSITORY git@github.com:mariadb-corporation/mariadb-connector-cpp.git
  GIT_TAG        1.1.6  # Or latest stable version
)

FetchContent_MakeAvailable(mariadb_cpp)

# MariaDBCPP uses CMAKE_SOURCE_DIR instead of PROJECT_SOURCE_DIR so have to set include paths manually...
target_include_directories(mariadbcpp_obj PRIVATE 
    ${mariadb_connector_cpp_SOURCE_DIR}/include
    ${mariadb_connector_cpp_SOURCE_DIR}/include/conncpp
    ${mariadb_connector_cpp_SOURCE_DIR}/src
    ${mariadb_connector_cpp_SOURCE_DIR}/class
    ${mariadb-connector-c_SOURCE_DIR}/include
    ${mariadb-connector-c_BINARY_DIR}/include
)


if (NPCS_BUILD_TESTS)
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
    add_executable(npcs_tests ${PROJECT_SOURCE_DIR}/tests/tests.cpp  ${PROJECT_SOURCE_DIR}/api/Utils.cpp)
    file(COPY ${PROJECT_SOURCE_DIR}/tests/sim/test_battles DESTINATION ${CMAKE_BINARY_DIR})
    
    add_test(NAME Tests COMMAND npcs_tests)
    target_link_libraries(npcs_tests npcs_sim gtest_main OpenSSL::Crypto)
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lib/json/single_include 
    ${PROJECT_SOURCE_DIR}/lib/tfhttp/include 
    ${PROJECT_SOURCE_DIR}/lib/tfhttp/lib/tflib/include 
    ${mariadb_connector_cpp_SOURCE_DIR}/include
)


find_package(OpenSSL REQUIRED)


target_link_libraries(npcs_sim TFLib)

target_link_libraries(npcshowdown PRIVATE
    tfhttp 
    TFLib 
    OpenSSL::Crypto
    npcs_sim
    mariadbcpp-static
    mariadbclient
)

