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
extern bool SLIDE;
extern bool WIZMOVED;
extern circularq WIZARDTILES;
extern Tile currentWizard;
extern int SCORE;

enum dist { man, euc, mink, cheb };
extern dist distanceType;

void printBoard(Board board);

#endif