#ifndef ALGORITHM_HELPER_H
#define ALGORITHM_HELPER_H

#define DEBUG false
#include<list>
#include"../structure/board.h"
#include"../structure/node_state.h"
#include"../logic/helper_functions.h"


using namespace std;

//manhattan distance
int distance(Tile t1, Tile t2){
    //manhattan distance - we can only move in 4 directions
    return abs(t1.getRow()-t2.getRow()) + abs(t1.getCol()-t2.getCol());
}
/* 
//eculidean distance
int distance(Tile t1, Tile t2){
    
    return sqrt((t1.getRow()-t2.getRow())*(t1.getRow()-t2.getRow()) +(t1.getCol()-t2.getCol())*(t1.getCol()-t2.getCol()));
}
 */

Board getPath(Node_State* win) {
    Board path = win->getBoard();
    Node_State* current = win;

    std::cout << "Tracing back the path...\n";
    while (current != nullptr) {
        Tile playerTile = current->getBoard().getPlayerTile();
        //std::cout << "DBG: Adding tile (" << playerTile.getRow() << ", " << playerTile.getCol() << ") to the path.\n";
        path.getTile(playerTile.getRow(), playerTile.getCol()).setValue(8);
        current = current->getParent();
    }

    std::cout << "Path tracing completed.\n";
    return path;
}

bool isVisited(list<Board> visitedStates, Board state){

    for(auto vState:visitedStates){

        if(isEqual(vState, state)){
            return true;
        }
    }

    return false;

}

string stringBoard(Board& b) {
    string boardHash;
    int n = b.getN();
    int m = b.getM();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            boardHash += to_string(b.getTile(i, j).getValue());
            boardHash += ',';
        }
    }
    return boardHash;
}

int calculate_cost(Node_State node) {
    Node_State* parent = node.getParent();
    if (parent == nullptr) {
        return node.getG(); // If root node, return its g value
    }
    return node.getG() + parent->getG();
}

#endif