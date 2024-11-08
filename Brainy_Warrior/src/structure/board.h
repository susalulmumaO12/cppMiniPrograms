#ifndef BOARD_H
#define BOARD_H

#include "tile.h"
#include <vector>

class Board {
private:
    int n, m;
    std::vector<std::vector<Tile>> board;

public:
    Board(int n, int m);

    int getN();
    int getM();

    Tile& getTile(int i, int j); 
    const Tile& getTile(int i, int j) const;
};

#endif
