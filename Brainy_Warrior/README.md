Brainy Warrior
==============

[Brainy Warrior](https://www.coolmathgames.com/0-brainy-warrior) is a puzzle game, the goal of the game is to defeat all enemies, by moving (sliding) towards them, if you don't have an enemy to defeat or a wall to stop you from running, you'll fall right into the water.

**Table of Contents:**
- [Game Structure](#game-structure)
- [Preview](#preview)
- [How to build](#how-to-build)
- [Code Breakdown](#code-breakdown)
- [Resources](#resources)
- [Draft of the thinking process](#draft-of-the-thinking-process)


## Game Structure

The game structure is split into folders for clean and easy maintnance, under `src` folder there exists: `structure` folder, which contains board.cpp, tile.cpp...etc.

While `logic` folder contains the functionalities for the game to run, which make the user able to play the game.

And finally, `game_solver` folder, which contains some algorithms that solve some levels of the game, such as BFS, DFS, A_Star and more.

## Preview

The game is printed on the console:

<img src="./screenshot/lvl1.png" alt="level 1" height="250" /> <img src="./screenshot/lvl2.png" alt="level 2" height="250" />
<br>
<img src="./screenshot/win.png" alt="win state" height="250" /> <img src="./screenshot/gameover.png" alt="lose state" height="250" />

_In the picture above_ `5` _is considered a target/ enemy and_ `9` _is the player._

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

## Code Breakdown


Movement functions are split into: canMove-, move-, slide-.

**canMove-** functions check for walls and edges of the board but they do not consider neighboring sea an invalid move. So they should be used in user movement functions.

**Move** functions operate in the following way:
```
    canMove-()? move-: pop up "invalid move!";
    move-{
        if next is sea: game over!;
        else return newBoard;
    }
```
While **slide** functions are different:
```
    canMove-()? slide- : pop up "invalid move!";
    slide-{
        if sea in way: game over!;
        else if target in way: replace target tile;
        else: slide-;
    }
```
The difference is: _slide_ function keeps checking the next tile until it meets a stop condition, but _move_ function only moves one tile at a time.

## Resources

- [nlohmann github repository](https://github.com/nlohmann/json)
- [ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code)
- [Working with Conan and CMake](https://www.codeproject.com/Articles/5385907/Managing-Cplusplus-Projects-with-Conan-and-CMake)

## Draft of the thinking process

<img src="./screenshot/draft.jpg" alt="level 1" height="500" />
