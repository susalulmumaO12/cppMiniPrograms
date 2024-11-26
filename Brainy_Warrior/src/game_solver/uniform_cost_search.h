#ifndef UNIFORM_COST_SEARCH_H
#define UNIFORM_COST_SEARCH_H

#include<iostream>
#include<queue>
#include<vector>
#include<list>
#include<climits>
#include<unordered_map>
#include<string>
#include"../structure/board.h"
#include"../structure/node_state.h"
#include"../structure/tile.h"
#include"../include/main.h"
#include"algorithm_helper.h"

using namespace std;

void ucs(Board board){
    priority_queue<Node_State> pq;

    Node_State start(board, nullptr, 0, 0);

    unordered_set<string> visitedStates;

    pq.push(start);
    visitedStates.insert(stringBoard(board));

    while(!pq.empty()){
        Node_State current = pq.top();
        printBoard(current.getBoard());
        pq.pop();

        if(current.getBoard().win()){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            cout<<"Number of opened states: "<<visitedStates.size()<<endl;
            cout<<"Path:\n";
            //printBoard(getPath(&current));
            exit(0);
        }

        list<Node_State> states = get_next_cost_states(&current);
        for(auto& state:states){
            string stateHash = stringBoard(state.getBoard());
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                Node_State newState(state.getBoard(), &current, calculate_cost(state), 0);
                pq.push(newState);
            }
        }

    }


}

#endif