#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H

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

void hill_climbing(Board board){
    priority_queue<Node_State> pq;

    Node_State start(board, nullptr, 0, 0);

    unordered_set<string> visitedStates;

    pq.push(start);
    visitedStates.insert(stringBoard(board));

    while(!pq.empty()){
        Node_State current = pq.top();
        printBoard(current.getBoard());
        pq.pop();

        Tile player = current.getBoard().getPlayerTile();
        
        vector<pair<int ,Tile>> targets;
        for (int i = 0; i < board.getN(); ++i) {
            for (int j = 0; j < board.getM(); ++j) {
                if (board.getTile(i, j).getValue() == 5) {
                    targets.emplace_back(distance(player, board.getTile(i,j)), board.getTile(i,j));
                }
            }
        } 

        auto minTarget = std::min_element(targets.begin(), targets.end(),
                                           [](const pair<int, Tile>& a, const pair<int, Tile>& b) {
                                               return a.first < b.first;
                                           });
        Tile goal = minTarget->second;

        if(current.getBoard().win()){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            cout<<"Number of opened states: "<<visitedStates.size()<<endl;
            exit(0);
        }

        list<Node_State> states = get_next_cost_states(&current);
        for(auto& state:states){
            string stateHash = stringBoard(state.getBoard());
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                Node_State newState(state.getBoard(), &current, 0, distance(goal, player));
                pq.push(newState);
            }
        }

    }

}

#endif