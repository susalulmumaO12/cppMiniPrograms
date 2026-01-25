#include "algorithms.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_set>
#include <stack>
#include <list>
#include "node_state.h"
#include "board.h"
#include "helper_functions.h"
#include "main.h"
#include "board_view.h"

using namespace std;


void dfs(Board board){

    stack<shared_ptr<Node_State>> s;
    unordered_set<string> visitedStates;

    auto start = make_shared<Node_State>(board, nullptr, 0, 0);
    s.push(start);
    visitedStates.insert(stringBoard(board));  

    while(!s.empty()){

        shared_ptr<Node_State> current = s.top();
        clear();
        printBoard(current->getBoard());
        mvprintw(LINES - 1, (COLS - 15) / 2, "DFS: Solving...");
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        s.pop();

        if(current->getBoard().win()){
            clear();
            printBoard(current->getBoard());
            mvprintw(LINES - 3, (COLS - 25) / 2, "YOU WIN! States: %zu", visitedStates.size());
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
        for(auto state:states){
            string stateHash = stringBoard(state->getBoard());
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                s.push(state);
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
