#ifndef BOARD_H
#define BOARD_H

#include "tile.h"
#include <vector>

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
    const Tile& getTile(int i, int j) const;

    bool win();
    void setTile(Tile& tile, int value);
};

#endif
