#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H
#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <string>
#include "../structure/node_state.h"
#include "../structure/board.h"
#include "../structure/tile.h"
#include "main.h"
#include "moves.h"

Board set_game_board(const std::string& levelKey);
void printTile(int value);
void printBoard(Board board);
void printStats(std::string& levelName);
void updateStats(const std::string& playerName, std::string& levelName, bool win);
dist getDistanceType();
std::list<Node_State> get_next_cost_states(Node_State* board);
int distance(Tile t1, Tile t2);
Board getPath(Node_State* win);
std::string stringBoard(Board& b);

#endif /* HELPER_FUNCTIONS.H */