Brainy Warrior
==============

[Brainy Warrior](https://www.coolmathgames.com/0-brainy-warrior) is a puzzle game, the goal of the game is to defeat all enemies, by moving (sliding) towards them, if you don't have an enemy to defeat or a wall to stop you from running, you'll fall right into the water.


## Game Structure

The game structure is split into folders for clean and easy maintnance, `structure` folder contains board.cpp, tile.cpp and state_node.cpp.

While `logic` folder will contain the functionalities for the game to run, which make the user able to play the game.

And finally, `game_solver` folder, which will contain some algorithms that solve some levels of the game, such as BFS, DFS, A_Star and more.


## How to build

Conan package manager and CMake are used for this process, to build the project run the following commands in `./Brainy_Warrior` one by one:
```sh
conan install .
```
```sh
cd build
```
```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE="Release/generators/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Release
```
```sh
cmake --build .
```
The C++ code should compile successfully, make sure by starting the game, run the following command (inside `./build` directory):
```sh
./Brainy_Warrior
```