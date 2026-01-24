#include "algorithms.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
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

struct NodeStatePtrComparator {
    bool operator()(const shared_ptr<Node_State>& a, const shared_ptr<Node_State>& b) const {
        return *a < *b;
    }
};

void ucs(Board board){
    priority_queue<shared_ptr<Node_State>, vector<shared_ptr<Node_State>>, NodeStatePtrComparator> pq;

    auto start = make_shared<Node_State>(board, nullptr, 0, 0);

    unordered_set<string> visitedStates;

    pq.push(start);
    visitedStates.insert(stringBoard(board));

    while(!pq.empty()){
        shared_ptr<Node_State> current = pq.top();
        clear();
        printBoard(current->getBoard());
        mvprintw(LINES - 1, (COLS - 25) / 2, "UCS: Solving... Cost: %d", current->getG());
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        pq.pop();

        if(current->getBoard().win()){
            clear();
            printBoard(current->getBoard());
            mvprintw(LINES - 3, (COLS - 35) / 2, "YOU WIN! States: %zu, Cost: %d", visitedStates.size(), current->getG());
            refresh();

            std::this_thread::sleep_for(std::chrono::seconds(1));
            clear();
            printBoard(getPath(current));
            mvprintw(LINES - 2, (COLS - 11) / 2, "Path Found!");
            mvprintw(LINES - 1, (COLS - 25) / 2, "Press any key to continue...");
            refresh();
            getch();
            return;
        }

        list<shared_ptr<Node_State>> states = get_next_cost_states(current);
        for(auto& state:states){
            string stateHash = stringBoard(state->getBoard());
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                pq.push(state);
            }
        }

    }

    // No path found
    clear();
    mvprintw(LINES / 2, (COLS - 20) / 2, "No path found.");
    mvprintw(LINES / 2 + 2, (COLS - 30) / 2, "Press any key to return...");
    refresh();
    getch();
}
