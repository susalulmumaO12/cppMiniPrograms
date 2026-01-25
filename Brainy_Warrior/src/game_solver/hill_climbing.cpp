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
#include <unordered_set>
#include "board.h"
#include "node_state.h"
#include "tile.h"
#include "helper_functions.h"
#include "main.h"
#include "board_view.h"


using namespace std;

int cost = 0;

struct NodeStatePtrComparator {
    bool operator()(const shared_ptr<Node_State>& a, const shared_ptr<Node_State>& b) const {
        return *a < *b;
    }
};

void hill_climbing(Board board) {
    priority_queue<shared_ptr<Node_State>, vector<shared_ptr<Node_State>>, NodeStatePtrComparator> pq;

    auto start = make_shared<Node_State>(board, nullptr, 0, 0);
    unordered_set<string> visitedStates;

    pq.push(start);
    visitedStates.insert(stringBoard(board));

    while(!pq.empty()){
        shared_ptr<Node_State> current = pq.top();
        clear();
        printBoard(current->getBoard());
        mvprintw(LINES - 1, (COLS - 35) / 2, "Hill Climbing: Solving... H: %d", current->getH());
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        pq.pop();

        Tile player = current->getBoard().getPlayerTile();
        
        list<Tile> targets = current->getBoard().getTargetTiles();

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
        if (current->getBoard().win()) {
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

        // Generate next states
        list<shared_ptr<Node_State>> states = get_next_cost_states(current);
        for (auto& state : states) {
            string stateHash = stringBoard(state->getBoard());
            Tile nextPlayer = state->getBoard().getPlayerTile();
            if (visitedStates.find(stateHash) == visitedStates.end()) {
                visitedStates.insert(stateHash);
                int h = distance(nextPlayer, goal);
                cost += state->getG();
                auto newState = make_shared<Node_State>(state->getBoard(), current, 0, current->getH() + h);
                pq.push(newState);
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
