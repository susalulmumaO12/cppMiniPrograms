#ifndef BOARD_H
#define BOARD_H

#include "tile.h"
#include <vector>
#include <iostream>
#include <list>

class Board {
private:
    int n, m;
    std::vector<std::vector<Tile>> board;
    bool wizardUsed = false;
    std::vector<Tile> wizardTiles;
    int currentWizardIndex = -1;

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
    
    bool isWizardUsed() const;
    void setWizardUsed(bool used);
    const std::vector<Tile>& getWizardTiles() const;
    void setWizardTiles(const std::vector<Tile>& tiles);
    int getCurrentWizardIndex() const;
    void setCurrentWizardIndex(int index);
};

#endif
