#ifndef BREADTH_FIRST_SEARCH_H
#define BREADTH_FIRST_SEARCH_H

#include<iostream>
#include<unordered_set>
#include<queue>
#include"../structure/board.h"
#include"../logic/helper_functions.h"
#include"../include/main.h"
#include"algorithm_helper.h"

void bfs(Board board){

    queue<Node_State> q;
    unordered_set<string> visitedStates;
    Node_State start(board, nullptr, 0, 0);
    q.push(start);
    visitedStates.insert(stringBoard(board));  

    while(!q.empty()){

        Node_State current = q.front();
        printBoard(current.getBoard());
        q.pop();

        if(current.getBoard().win()){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            exit(0);
        }

        list<Node_State> states = get_next_cost_states(&current);
        for(auto state:states){
            string stateHash = stringBoard(state.getBoard());
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                q.push(state);
            }
        }

    }
}

#endif