#ifndef MOVES_H
#define MOVES_H

#include <iostream>
#include <string>
#include <list>
#include "../structure/board.h"
#include "../structure/tile.h"
#include "main.h"


void swapTiles(Board& b, Tile& tile1, Tile& tile2);
bool canMoveUp(Board b);
bool canMoveDown(Board b);
bool canMoveLeft(Board b);
bool canMoveRight(Board b);
Board moveUp(Board& b);
Board moveDown(Board& b);
Board moveLeft(Board& b);
Board moveRight(Board& b);
Board slideUp(Board& b);
Board slideDown(Board& b);
Board slideLeft(Board& b);
Board slideRight(Board& b);
circularq getWizardTiles(Board& board);
Board move(Board& b, char m);
Board moveWizard(Board& board);

#endif /* MOVES.H */