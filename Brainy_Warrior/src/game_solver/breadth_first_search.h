#ifndef BREADTH_FIRST_SEARCH_H
#define BREADTH_FIRST_SEARCH_H

#include<iostream>
#include<list>
#include<queue>
#include"../structure/board.h"
#include"../include/main.h"
#include"algorithm_helper.h"

void bfs(Board board){

    queue<Board> q;
    q.push(board);
    list<Board> visitedStates;

    visitedStates.push_back(board);

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

            if(!isVisited(visitedStates, state)){
                visitedStates.push_back(state);
                q.push(state);
            }
        }

    }
}

#endif