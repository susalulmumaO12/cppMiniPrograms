#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include<iostream>
#include<unordered_set>
#include<stack>
#include"../structure/board.h"
#include"../include/main.h"
#include"algorithm_helper.h"


void dfs(Board board){

    stack<Board> s;
    unordered_set<string> visitedStates;

    s.push(board);
    visitedStates.insert(stringBoard(board));  

    while(!s.empty()){

        Board current = s.top();
        printBoard(current);
        s.pop();

        if(current.win()){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            exit(0);
        }

        list<Board> states = get_next_states(current);
        for(auto state:states){
            string stateHash = stringBoard(state);
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                s.push(state);
            }
        }
    }
}

#endif