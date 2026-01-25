#include "algorithms.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <unordered_set>
#include <queue>
#include <list>
#include "node_state.h"
#include "board.h"
#include "helper_functions.h"
#include "main.h"
#include "board_view.h"

using namespace std;


void bfs(Board board){

    queue<shared_ptr<Node_State>> q;
    unordered_set<string> visitedStates;
    auto start = make_shared<Node_State>(board, nullptr, 0, 0);
    q.push(start);
    visitedStates.insert(stringBoard(board));  

    int explored = 0;
    while(!q.empty()){

        shared_ptr<Node_State> current = q.front();
        q.pop();
        explored++;

        if (explored % 100 == 0) {
            clear();
            printBoard(current->getBoard());
            mvprintw(LINES - 1, (COLS - 30) / 2, "BFS: Solving... Explored: %d", explored);
            refresh();
        }

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
                q.push(state);
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
