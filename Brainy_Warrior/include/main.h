#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
#include "../src/structure/board.h"
#include "../src/structure/tile.h"

struct circularq {
    std::vector<Tile> tiles;
    int front = 0;

    void push(Tile t){
        tiles.emplace_back(t);
    }

    Tile nextTile(){
        front = (front +1) % tiles.size();
        return tiles[front];
    }
};

//slide or move tile by tile
bool SLIDE = false;
bool WIZMOVED = false;
circularq WIZARDTILES;
Tile currentWizard;

enum dist { man, euc, mink, cheb };

void printBoard(Board board);

dist getDistanceType();
dist distanceType;

#endif