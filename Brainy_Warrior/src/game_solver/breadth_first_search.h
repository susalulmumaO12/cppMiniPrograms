#ifndef BREADTH_FIRST_SEARCH_H
#define BREADTH_FIRST_SEARCH_H

#include<iostream>
#include<unordered_set>
#include<queue>
#include"../structure/board.h"
#include"../include/main.h"
#include"algorithm_helper.h"

void bfs(Board board){

    queue<Board> q;
    unordered_set<string> visitedStates;

    q.push(board);
    visitedStates.insert(stringBoard(board));  

    while(!q.empty()){

        Board current = q.front();
        printBoard(current);
        q.pop();

        if(current.win()){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            exit(0);
        }

        list<Board> states = get_next_states(current);
        for(auto state:states){
            string stateHash = stringBoard(state);
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                q.push(state);
            }
        }

    }
}

#endif