#include "algorithms.h"

#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <climits>
#include <unordered_set>
#include <string>
#include "board.h"
#include "node_state.h"
#include "tile.h"
#include "helper_functions.h"
#include "main.h"


using namespace std;

void ucs(Board board){
    priority_queue<Node_State> pq;

    Node_State start(board, nullptr, 0, 0);

    unordered_set<string> visitedStates;

    pq.push(start);
    visitedStates.insert(stringBoard(board));

    while(!pq.empty()){
        Node_State current(pq.top());
        printBoard(current.getBoard());
        pq.pop();

        if(current.getBoard().win()){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            cout<<"Number of opened states: "<<visitedStates.size()<<endl;
            cout<< "cost: " << current.getF()<<endl;
            cout<<"Path:\n";
            printBoard(getPath(&current));
            exit(0);
        }

        list<Node_State> states = get_next_cost_states(&current);
        for(auto& state:states){
            string stateHash = stringBoard(state.getBoard());
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                Node_State newState(state.getBoard(), &current, state.getG(), 0);
                pq.push(newState);
            }
        }

    }


}
