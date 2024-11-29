#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include "nlohmann/json.hpp"
#include "../src/structure/board.h"

enum dist {man, euc, diag};

void printBoard(Board board);

dist getDistanceType();
dist distanceType;

#endif