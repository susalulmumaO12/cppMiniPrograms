#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H

#include<iostream>
#include<queue>
#include<vector>
#include<list>
#include<climits>
#include<unordered_map>
#include<string>
#include<unordered_set>
#include"../structure/board.h"
#include"../structure/node_state.h"
#include"../structure/tile.h"
#include"../include/main.h"
#include"../logic/helper_functions.h"
#include"algorithm_helper.h"

using namespace std;

void hill_climbing(Board board) {
    priority_queue<Node_State> pq;

    Node_State start(board, nullptr, 0, 0);
    unordered_set<string> visitedStates;

    pq.push(start);
    visitedStates.insert(stringBoard(board));

    while(!pq.empty()){
        Node_State current(pq.top());
        printBoard(current.getBoard());
        pq.pop();

        Tile player = current.getBoard().getPlayerTile();
        
        list<Tile> targets = current.getBoard().getTargetTiles();

        // Find the closest target tile
        Tile goal;
        int minDistance = INT_MAX;
        for (const Tile& target : targets) {
            int dist = distance(player, target);
            if (dist < minDistance) {
                minDistance = dist;
                goal = target;
            }
        }

        // Check for win condition
        if (current.getBoard().win()) {
            cout << "\033[38;5;226mYOU WIN!\033[0m\n";
            cout << "Number of opened states: " << visitedStates.size() << endl;
            printBoard(getPath(&current));
            exit(0);
        }

        // Generate next states
        list<Node_State> states = get_next_cost_states(&current);
        for (auto& state : states) {
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
