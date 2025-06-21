<pre><strong>

        __________               .__                __      __                     .__              
        \______   \____________  |__| ____ ___.__. /  \    /  \_____ ______________|__| ___________ 
         |    |  _/\_  __ \__  \ |  |/    <   |  | \   \/\/   /\__  \\_  __ \_  __ \  |/  _ \_  __ \
         |    |   \ |  | \// __ \|  |   |  \___  |  \        /  / __ \|  | \/|  | \/  (  <_> )  | \/
         |______  / |__|  (____  /__|___|  / ____|   \__/\  /  (____  /__|   |__|  |__|\____/|__|   
                \/             \/        \/\/             \/        \/                              

</strong>
</pre>
[Brainy Warrior](https://www.coolmathgames.com/0-brainy-warrior) is a puzzle game, the goal of the game is to defeat all enemies, by moving (sliding) towards them, if there is no enemy or wall standing in your way, you might fall into the sea!

**Table of Contents:**

- [**Preview**](#preview)
- [**Game Flow**](#game-flow)
- [**How to Run**](#how-to-run)
    - [Requirements](#requirements)
    - [Build](#build)
- [**Code Breakdown**](#code-breakdown)
    - [Game Structure](#game-structure)
    - [Movement Functions](#movement-functions)
    - [Algorithms](#algorithms)
    - [Heuristics Calculation](#heuristics-calculation)
    - [Path Retrieval](#path-retrieval)
    - [Wizard Movement](#wizard-movement)
- [**Scoring System and Stats**](#scoring-system-and-stats)
- [**Resources**](#resources)
- [**Draft of the thinking process**](#draft-of-the-thinking-process)
- [**Future Improvements**](#future-improvements)
- [**Old Looks**](#old-looks)

## Preview

The game is printed on the console, and takes user input from keyboard on game options:

<img src="./screenshot/lvl1.png" alt="level 1" height="150" /> <img src="./screenshot/gameover.png" alt="lose state" height="150" />
<br>
<img src="./screenshot/win.png" alt="win state" height="150" />&emsp; <img src="./screenshot/path.png" alt="path" height="150" />

_In the pictures above_ `5` _is considered a target/ enemy and_ `9` _is the player._

## Game Flow
<!-- 
TODO screenshots of user prompts
-->
Below is a flowchart that demonstrates the game flow ([display full size](./screenshot/flowchart.png)):

<img src="./screenshot/flowchart.png" height="500" alt="flowchart"/>

## How to Run

_Tested on Linux_

### Requirements

- C++ 17 Compiler
- CMake >= v3.12
- Conan
- Ncurses with wide-char/UTF-8 support
- Terminal emulator with UTF-8 support

### Build

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

### Game Structure

The game utilizes a 2D board of tiles to maintain state in general. A `Tile` is an object of **{row, col, value}**. A `Board` is a 2D vector of type `Tile`, and `Node_State` is an object of **{Board, Parent, G, H, F}**, used for implementing algorithms.

### Movement Functions

Movement function `move` takes in either WASD or IJKL to process the direction of movement.

A directed movement function `moveX` (where X is the direction), checks if movement is valid, by calling `canMoveX`, then if valid it does the following:

```pseudo
next = 0? // sea
    player = -1 // GAME OVER
next = 1? // land
    swap(player, next)
next = 5, 7? // enemy, wizard
    swap(player, next)
    next = 7? WIZARDUSED = true // player can no longer use wizard
```

> [!NOTE]
> A valid movement means there's no wall blocking the way, hence player might fall into the sea.

### Algorithms

> [!IMPORTANT]
> _any explanation on the algorithms below is specific to my implementation and is not necessarily accurate._

#### Breadth-First-Search:

**BFS** function takes in a _board_, and generates costless states and itarates over them in a queue until _win_ state is met, and maintains an unordered set of stringBoard to avoid revisiting the same node, **stringBoard** is basically the board as a **1D** array of chars put into a string row by row.

#### Depth-First-Search:

**DFS** function also takes in a _board_, the only difference in implementation between DFS and BFS is that it uses a stack to store the generated costless states and _itarates_ over them (not recursively).

#### Uniform-Cost-Search:

**UCS** implementation is pretty similar to BFS, as it also uses a queue, key difference is that it's a priority queue (min heap), hence, it generates costly states, with a different value for each direction of movement.

#### Simple Hill Climbing:

**Simple Hill Climbing** is very similar to **UCS** except for one key difference, instead of comparing over the g value (cost from start to current node) in the priority queue, we compare over the heuristic value, in the basic implementation I used **Manhattan Distance** formula, **Euclidean** method can also be used but it ignores the movement style in our game.

#### A_Star-Search:

**A\*** or **A_Star** is implemented with a priority queue and an unordered map, key point difference from uniform cost search is that it calculates heuristic and not just cost, there are many methods to achieve A_Star in our game, I chose the simplest one, starting with the closest target and with each movement the closest target is updated.

### Heuristics Calculation

> [!NOTE]
> The game restricts movement to 4 directions, Manhattan Distance is the most logical way to calculate distance, the other methods are experimental only (for educational purposes).

When user selects computer as the player, the game prompts the user to choose heuristic calculation method, `distance` function takes two parameters of type `Tile`, `t1` and `t2`, here are the implemented methods so far:

#### Manhattan Distance:

Measures the total grid-based distance (only horizontal and vertical moves are allowed).

<div align="center"><img src="https://www.maartengrootendorst.com/images/posts/2021-01-02-distances/manhattan.svg" width="150" style="background:white"></div>

$$
\text{D} = |x_1 - x_2| + |y_1 - y_2| 
$$
$$
\text{D(x, y)} = \sum_{i=1}^k |x_i - y_i|
$$

```cpp
abs(t1.getRow()-t2.getRow()) + abs(t1.getCol()-t2.getCol());
```
#### Euclidean Distance:

Measures the straight-line distance between two points.

<div align="center"><img src="https://www.maartengrootendorst.com/images/posts/2021-01-02-distances/euclidean.svg" width="150" style="background:white"></div>

$$
\text{D} = \sqrt{(x_1 - x_2)^2 + (y_1 - y_2)^2}
$$
$$
\text{D(x, y)} = \sqrt{\sum_{i=1}^n (x_i - y_i)^2}
$$
```cpp
sqrt((t1.getRow()-t2.getRow())*(t1.getRow()-t2.getRow()) +(t1.getCol()-t2.getCol())*(t1.getCol()-t2.getCol()));
```
#### Minkowski Distance:

A generalized distance metric where Manhattan and Euclidean distances are special cases (`p=1` and `p=2`, respectively).
Formula: 

<div align="center"><img src="https://www.maartengrootendorst.com/images/posts/2021-01-02-distances/minkowski.svg" width="150" style="background:white"></div>

$$
D = \left( \sum_{i=1}^n |x_i - y_i|^p \right)^{1/p}
$$

```cpp
double sum = 0.0;
for (size_t i = 0; i < v1.size(); ++i) {
    sum += pow(abs(v1[i] - v2[i]), minkowskiP);
}
return static_cast<int>(pow(sum, 1.0 / minkowskiP)); // P = 3
```

#### Chebyshev Distance:

The Chebyshev distance calculation, commonly known as the "maximum metric" in mathematics, measures distance between two points as the maximum difference over any of their axis values. In a 2D grid, for instance, if we have two points (x1, y1), and (x2, y2), the Chebyshev distance between is max(y2 - y1, x2 - x1).

<div align="center"><img src="https://www.maartengrootendorst.com/images/posts/2021-01-02-distances/chebyshev.svg" width="150" style="background:white"></div>

$$
\text{D} = \max_{i}(|x_i - y_i|)
$$

```cpp
max((t2.getRow() - t1.getRow(), (t2.getCol() - t1.getCol())));
```

### Path Retrieval

To print the final path that the algorithm got from start node to end node (win state), the final state is passed to the `getPath` function, then it checks for player position in each iteration while accessing the parent node of each current node, resulting in this:


<img src="./screenshot/lvl5.png" height="150" alt="path"/>&emsp;**[>]**&emsp;<img src="./screenshot/path.png" height="150" alt="path"/>

_Pictures above show the path of BFS on level 5_

### Wizard Movement

When a board is initialized, Wizard tiles are collected for later use, they're put in a circular queue, and the Wizard moves upon that, unless the player has used up the Wizard, they can still move the Wizard clockwise.

## Scoring System and Stats

Scoring system only keeps track for every level overtime, and keeps stats for each player like number of times played and number of wins, like this:

<table>
<thead>
<th colspan="4" text-align="center">Level 3</th>
</thead>
<tbody>
<tr><th>Name</th><th>Score</th><th>Tries</th><th>Wins</th></tr>
<tr><td>Alice</td><td>3</td><td>1</td><td>1</td></tr>
<tr><td>Bob</td><td>7</td><td>2</td><td>2</td></tr>
<tr><td>Carol</td><td>15</td><td>5</td><td>1</td></tr>
</tbody>
</table>

_Numbers above are trivial_

_Highest score is in fact the least score, it indicates number of moves_

## Resources

**General:**
- [nlohmann github repository](https://github.com/nlohmann/json)
- [ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code)
- [Working with Conan and CMake](https://www.codeproject.com/Articles/5385907/Managing-Cplusplus-Projects-with-Conan-and-CMake)
- [ASCII art Generator (for the banner)](https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Brainy%20Warrior)
- [Alice and Bob](https://en.wikipedia.org/wiki/Alice_and_Bob)

**NCurses:**
- [NCurses Doc](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html)
- [FinalCut C++ Library for TUI](https://github.com/gansm/finalcut): I did not use it but it's worth checking for lightweight C++ library for creating terminal-based applications.
- [Curses (ncurses) tutorial](https://github.com/mcdaniel/curses_tutorial)
- [Colors in NCurses](https://www.linuxjournal.com/content/about-ncurses-colors-0)
- [Custom colors](https://www.youtube.com/watch?v=YyyhQ0B3huo&ab_channel=CasualCoder): Though might not work based on your terminal emulator, if it does not implement `initc` feature then you won't see the changes (further read [here](https://stackoverflow.com/questions/10809350/ncurses-why-does-init-color-return-ok-but-still-not-set-the-color)).

**Algorithms:**
- [A* (A-Star) algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm)
- [A* by Patrick Lester](https://web.archive.org/web/20051230012332/https://www.policyalmanac.org/games/aStarTutorial.htm)
- [Hill Climbing in AI](https://www.geeksforgeeks.org/introduction-hill-climbing-artificial-intelligence/)

**Worth Reading papers:**
- [Multi-Target Pathfinding: Evaluating A-star Versus BFS](http://mau.diva-portal.org/smash/get/diva2:1897067/FULLTEXT02.pdf)
- [9 Distance Measures in Data Science](https://www.maartengrootendorst.com/blog/distances/)
- [Measuring Distance](https://chris3606.github.io/GoRogue/articles/grid_components/measuring-distance.html)

## Draft of the thinking process

<img src="./screenshot/page1.png" alt="level 1" height="300" /> <img src="./screenshot/page2.png" alt="level 1" height="300" /> <img src="./screenshot/page3.png" alt="level 1" height="300" /> <img src="./screenshot/wizard.png" height="300" />

## Future Improvements

There are various ways to improve the game which is currently under construction:

- [ ] Make TUI, using ncurses possibly.
- ~~[ ] Optimize time complexity of computer playing~~
- [X] Implement levels with wizard (above 5 on the original game website)
- [X] Track score for players based on name, and number of steps
- [X] Print scores and stats
- [X] Main-menu
- [X] Print path for user

## Old Looks

<img src="./screenshot/lvl1old.png" alt="level 1" height="100" /> <img src="./screenshot/lvl2old.png" alt="level 2" height="100" /> <img src="./screenshot/winold.png" alt="win state" height="100" /> <img src="./screenshot/gameoverold.png" alt="lose state" height="100" />
