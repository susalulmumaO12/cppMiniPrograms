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
#endif