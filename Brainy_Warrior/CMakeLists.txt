# Set project properties
cmake_minimum_required(VERSION 3.12)
project(brainy_warrior_project LANGUAGES CXX)

# Set directory variables
set(SOURCE_DIR src)
set(STRUCTURE_DIR src/structure)
set(LOGIC_DIR src/logic)
set(GAME_SOLVER_DIR src/game_solver)
set(INCLUDE_DIR include)

# Access JSON package
find_package(nlohmann_json REQUIRED)

# Define target
add_executable(Brainy_Warrior 
    ${SOURCE_DIR}/main.cpp 
    ${STRUCTURE_DIR}/board.cpp
    ${STRUCTURE_DIR}/node_state.cpp
    ${STRUCTURE_DIR}/tile.cpp
    ${LOGIC_DIR}/helper_functions.h
    ${LOGIC_DIR}/moves.h
    ${GAME_SOLVER_DIR}/breadth_first_search.h
    ${GAME_SOLVER_DIR}/depth_first_search.h
    ${GAME_SOLVER_DIR}/algorithm_helper.h
    ${GAME_SOLVER_DIR}/uniform_cost_search.h
    ${GAME_SOLVER_DIR}/A_star_search.h
)

#include headers
include_directories(${STRUCTURE_DIR} ${LOGIC_DIR} ${GAME_SOLVER_DIR})

# Set target properties
target_compile_options(Brainy_Warrior PUBLIC -Wall -std=c++17)
target_link_libraries(Brainy_Warrior nlohmann_json::nlohmann_json)
target_include_directories(Brainy_Warrior PUBLIC ${INCLUDE_DIR})