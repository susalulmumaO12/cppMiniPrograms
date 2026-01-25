#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <string>
#include <memory>
#include "node_state.h"
#include "board.h"
#include "tile.h"
#include "main.h"
#include "moves.h"

Board set_game_board(std::string levelKey);
std::vector<std::pair<int, std::pair<int, std::pair<int, std::string>>>> fetchStats(std::string levelName);
void updateStats(const std::string& playerName, std::string levelName, bool win);
dist getDistanceType();
std::list<std::shared_ptr<Node_State>> get_next_cost_states(std::shared_ptr<Node_State> board);
int distance(Tile t1, Tile t2);
Board getPath(std::shared_ptr<Node_State> win);
std::string stringBoard(Board& b);
void debug_log(const std::string& message);

#endif /* HELPER_FUNCTIONS.H */
