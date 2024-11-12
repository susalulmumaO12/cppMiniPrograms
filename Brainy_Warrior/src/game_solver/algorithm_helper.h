#ifndef ALGORITHM_HELPER_H
#define ALGORITHM_HELPER_H

#include"../structure/board.h"
#include"../logic/helper_functions.h"

using namespace std;

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
#endif