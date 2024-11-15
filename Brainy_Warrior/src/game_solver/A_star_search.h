#ifndef A_STAR_SEARCH_H
#define A_STAR_SEARCH_H

#include<iostream>
#include<priority_queue>
#include<vector>
#include"../structure/board.h"
#include"../structure/node_state.h"
#include"../include/main.h"

int distance(Tile t1, Tile t2){
    //manhattan distance - we can only move in 4 directions
    return abs(t1.getRow()-t2.getRow()) + abs(t1.getCol()-t2.getCol());
}
void a_star(Board board){
    priority_queue<Node_State> openList;
    vector<Node_State> closedList;

}
#endif