#ifndef A_STAR_SEARCH_H
#define A_STAR_SEARCH_H

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
#include"../include/main.h"
#include"algorithm_helper.h"

using namespace std;

void a_star(Board board) {
    priority_queue<Node_State> openList;
    unordered_map<string, bool> closedList;

    Tile player = board.getPlayerTile();
    list<Tile> targets = board.getTargetTiles();

    Node_State startNode(board, nullptr, 0, distance(player, targets.front()));
    openList.push(startNode);

    while (!openList.empty()) {
        Node_State currentNode = openList.top();
        openList.pop();

        Board currentBoard = currentNode.getBoard();
        string boardKey = stringBoard(currentBoard);

        if (currentBoard.win()) {
            cout << "\033[38;5;226mYOU WIN!\033[0m\n";
            cout << "Number of opened states: " << closedList.size() << endl;
            exit(0);
        }

        if (closedList[boardKey]) continue;
        closedList[boardKey] = true;
        printBoard(currentBoard);

        list<Board> nextStates = get_next_states(currentBoard);
        for (auto& nextBoard : nextStates) {
            Tile nextPlayer = nextBoard.getPlayerTile();
            // recalculate heuristic for each target
            for (const auto& target : targets) {
                int g = currentNode.getG() + 1;
                int h = distance(nextPlayer, target);
                Node_State nextNode(nextBoard, &currentNode, g, h);
                openList.push(nextNode);
            }
        }
    }
}

#endif