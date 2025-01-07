#include "algorithms.h"

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
#include"../logic/helper_functions.h"
#include"../include/main.h"


using namespace std;

void a_star(Board board) {
    priority_queue<Node_State> openList;
    unordered_map<string, bool> closedList;

    Tile player = board.getPlayerTile();
    list<Tile> targets = board.getTargetTiles();

    Node_State startNode(board, nullptr, 0, distance(player, targets.front()));
    openList.push(startNode);

    while (!openList.empty()) {
        Node_State current(openList.top());
        openList.pop();

        Board currentBoard = current.getBoard();
        string boardKey = stringBoard(currentBoard);

        if (currentBoard.win()) {
            cout << "\033[38;5;226mYOU WIN!\033[0m\n";
            cout << "Number of opened states: " << closedList.size() << endl;
            cout<< "cost: " << current.getF()<<endl;
            printBoard(getPath(&current));
            exit(0);
        }

        if (closedList[boardKey]) continue;
        closedList[boardKey] = true;
        printBoard(currentBoard);

        list<Node_State> nextStates = get_next_cost_states(&current);
        for (auto& nextState : nextStates) {
            Tile nextPlayer = nextState.getBoard().getPlayerTile();
            // recalculate heuristic for each target
            for (const auto& target : targets) {
                int g = nextState.getG();
                int h = distance(nextPlayer, target);
                Board nextBoard = nextState.getBoard();
                Node_State nextNode(nextBoard, &current, g, h);
                openList.push(nextNode);
            }
        }
    }
}