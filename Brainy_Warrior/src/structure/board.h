#ifndef BOARD_H
#define BOARD_H

#include "tile.h"
#include <vector>
#include<iostream>
#include <list>

class Board {
private:
    int n, m;
    std::vector<std::vector<Tile>> board;

public:
    Board(int n, int m);
    Board(const Board& other);

    int getN();
    int getM();

    Tile& getTile(int i, int j); 
    Tile& getPlayerTile();
    std::list<Tile> getTargetTiles();

    bool win();
    void setTile(Tile& tile, int value);
    
    /* //defining = operator
    Board operator=(const Board& other) const {
        return Board(other);
    } */
};

#endif
