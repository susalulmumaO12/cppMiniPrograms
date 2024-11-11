#ifndef DEPTH_FIRST_SEARCH_H
#define DEPTH_FIRST_SEARCH_H

#include<iostream>
#include<list>
#include<stack>
#include"../structure/board.h"
#include"../include/main.h"
#include"algorithm_helper.h"


void dfs(Board board){

    stack<Board> s;
    s.push(board);
    list<Board> visitedStates;

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

            if(!isVisited(visitedStates, state)){
                visitedStates.push_back(state);
                s.push(state);
            }
        }
    }
}

#endif