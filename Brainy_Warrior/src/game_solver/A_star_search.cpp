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

void a_star(Board board) {
    priority_queue<shared_ptr<Node_State>, vector<shared_ptr<Node_State>>, NodeStatePtrComparator> openList;
    unordered_map<string, int> closedList;

    list<Tile> targets = board.getTargetTiles();

    auto startNode = make_shared<Node_State>(board, nullptr, 0, 0);
    openList.push(startNode);
    closedList[stringBoard(board)] = 0;

    int explored = 0;
    while (!openList.empty()) {
        shared_ptr<Node_State> current = openList.top();
        openList.pop();

        Board& currentBoard = current->getBoard();
        string boardKey = stringBoard(currentBoard);

        if (closedList.count(boardKey) && current->getG() > closedList[boardKey]) {
            continue;
        }
        explored++;

        if (explored % 100 == 0) {
            clear();
            printBoard(currentBoard);
            mvprintw(LINES - 1, (COLS - 45) / 2, "A*: Solving... Explored: %d, F: %d, H: %d", explored, current->getF(), current->getH());
            refresh();
        }

        if (currentBoard.win()) {
            clear();
            printBoard(currentBoard);
            mvprintw(LINES - 3, (COLS - 30) / 2, "YOU WIN! States: %zu, F: %d", closedList.size(), current->getF());
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

        list<shared_ptr<Node_State>> nextStates = get_next_cost_states(current);
        for (auto& nextState : nextStates) {
            string nextKey = stringBoard(nextState->getBoard());
            int nextG = nextState->getG();

            if (closedList.find(nextKey) == closedList.end() || nextG < closedList[nextKey]) {
                Tile nextPlayer = nextState->getBoard().getPlayerTile();
                list<Tile> nextTargets = nextState->getBoard().getTargetTiles();
                
                int h = 0;
                if (!nextTargets.empty()) {
                    int minH = INT_MAX;
                    for (const auto& target : nextTargets) {
                        int d = distance(nextPlayer, target);
                        if (d < minH) minH = d;
                    }
                    
                    // GENERALIZED COMPLEXITY HEURISTIC (Phase 38):
                    // 1. Target count
                    // 2. Proximity of player to nearest target
                    
                    // Instead of hardcoding level numbers, we scale the heuristic 
                    // based on board features (like wizard tiles).
                    int wizardCount = nextState->getBoard().getWizardTiles().size();
                    
                    if (wizardCount > 0) {
                        // High-complexity level (likely requires setup moves)
                        // Use a lower target multiplier to encourage exploration
                        h = (nextTargets.size() * 5) + minH; 
                    } else {
                        // Simple level
                        h = (nextTargets.size() * 50) + minH;
                    }
                }

                auto nextNode = make_shared<Node_State>(nextState->getBoard(), current, nextG, h);
                closedList[nextKey] = nextG;
                openList.push(nextNode);
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
