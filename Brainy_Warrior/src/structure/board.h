#ifndef BOARD_H
#define BOARD_H

#include "tile.h"
#include <vector>
#include <list>

class Board {
private:
    int n, m;

public:
    Board(int n, int m);
    std::vector<std::vector<Tile>> board;

    int getN();
    int getM();

    Tile& getTile(int i, int j); 
    Tile& getPlayerTile();

    list<Tile&> getTargets();

    bool win();
    void setTile(Tile& tile, int value);
};

#endif
