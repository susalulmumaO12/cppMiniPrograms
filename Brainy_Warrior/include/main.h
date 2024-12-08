#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include "nlohmann/json.hpp"
#include "../src/structure/board.h"

//slide or move tile by tile
bool SLIDE = false;
bool WIZMOVED = false;

enum dist { man, euc, mink, cheb };

void printBoard(Board board);

dist getDistanceType();
dist distanceType;

#endif