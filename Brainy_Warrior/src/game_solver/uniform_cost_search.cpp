#include "algorithms.h"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <climits>
#include <unordered_map>
#include <string>
#include "board.h"
#include "node_state.h"
#include "tile.h"
#include "helper_functions.h"
#include "main.h"
#include "board_view.h"


using namespace std;

struct NodeStatePtrComparator {
    bool operator()(const shared_ptr<Node_State>& a, const shared_ptr<Node_State>& b) const {
        return *a < *b;
    }
};

void ucs(Board board){
    priority_queue<shared_ptr<Node_State>, vector<shared_ptr<Node_State>>, NodeStatePtrComparator> pq;

    auto start = make_shared<Node_State>(board, nullptr, 0, 0);

    unordered_map<string, int> closedList;

    pq.push(start);
    closedList[stringBoard(board)] = 0;

    int explored = 0;
    while(!pq.empty()){
        shared_ptr<Node_State> current = pq.top();
        pq.pop();

        Board& currentBoard = current->getBoard();
        string boardKey = stringBoard(currentBoard);

        // If we found a more expensive path to a known state, skip it
        if (closedList.count(boardKey) && current->getG() > closedList[boardKey]) {
            continue;
        }
        explored++;

        if (explored % 100 == 0) {
            clear();
            printBoard(currentBoard);
            mvprintw(LINES - 1, (COLS - 45) / 2, "UCS: Solving... Explored: %d, Cost: %d", explored, current->getG());
            refresh();
        }

        if(currentBoard.win()){
            clear();
            printBoard(currentBoard);
            mvprintw(LINES - 3, (COLS - 35) / 2, "YOU WIN! States: %zu, Cost: %d", closedList.size(), current->getG());
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
        for(auto& nextState:states){
            string stateHash = stringBoard(nextState->getBoard());
            int nextG = nextState->getG();
            
            if (closedList.find(stateHash) == closedList.end() || nextG < closedList[stateHash]) {
                closedList[stateHash] = nextG;
                pq.push(nextState);
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
