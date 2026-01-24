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


using namespace std;

struct NodeStatePtrComparator {
    bool operator()(const shared_ptr<Node_State>& a, const shared_ptr<Node_State>& b) const {
        return *a < *b;
    }
};

void a_star(Board board) {
    priority_queue<shared_ptr<Node_State>, vector<shared_ptr<Node_State>>, NodeStatePtrComparator> openList;
    unordered_map<string, bool> closedList;

    Tile player = board.getPlayerTile();
    list<Tile> targets = board.getTargetTiles();

    auto startNode = make_shared<Node_State>(board, nullptr, 0, distance(player, targets.front()));
    openList.push(startNode);

    while (!openList.empty()) {
        shared_ptr<Node_State> current = openList.top();
        openList.pop();

        Board& currentBoard = current->getBoard();
        string boardKey = stringBoard(currentBoard);

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

        if (closedList[boardKey]) continue;
        closedList[boardKey] = true;
        
        clear();
        printBoard(currentBoard);
        mvprintw(LINES - 1, (COLS - 25) / 2, "A*: Solving... F: %d", current->getF());
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        list<shared_ptr<Node_State>> nextStates = get_next_cost_states(current);
        for (auto& nextState : nextStates) {
            Tile nextPlayer = nextState->getBoard().getPlayerTile();
            list<Tile> nextTargets = nextState->getBoard().getTargetTiles();
            
            // Improved Heuristic for Multi-Target sliding games:
            // 1. Min distance to nearest target
            // 2. Plus a penalty for each remaining target to encourage clearing them
            int h = 0;
            if (!nextTargets.empty()) {
                int minH = INT_MAX;
                for (const auto& target : nextTargets) {
                    int d = distance(nextPlayer, target);
                    if (d < minH) minH = d;
                }
                h = minH + (nextTargets.size() * 2); // Encourages reduction of target count
            }

            auto nextNode = make_shared<Node_State>(nextState->getBoard(), current, nextState->getG(), h);
            openList.push(nextNode);
        }
    }

    // No path found
    clear();
    mvprintw(LINES / 2, (COLS - 20) / 2, "No path found.");
    mvprintw(LINES / 2 + 2, (COLS - 30) / 2, "Press any key to return...");
    refresh();
    getch();
}
