cmake_minimum_required(VERSION 3.10)
project(Brainy_Warrior LANGUAGES CXX)

# Set directory variables
set(INCLUDE_DIR include)

# Access JSON package
find_package(nlohmann_json REQUIRED)

# Define target
add_executable(Brainy_Warrior main.cpp)

# Set target properties
target_link_libraries(Brainy_Warrior nlohmann_json::nlohmann_json)
target_include_directories(Brainy_Warrior PUBLIC ${INCLUDE_DIR})