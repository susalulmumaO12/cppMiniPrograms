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

using namespace std;

Board set_game_board(int levelChoice);
void printScores(int level);
void updateStats(const string& playerName, int level, bool win);
list<Node_State> get_next_cost_states(Node_State* board);
int distance(Tile t1, Tile t2);
Board getPath(Node_State* win);
string stringBoard(Board& b);

#endif /* HELPER_FUNCTIONS.H */