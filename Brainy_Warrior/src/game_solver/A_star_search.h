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

int distance(Tile t1, Tile t2){
    //manhattan distance - we can only move in 4 directions
    return abs(t1.getRow()-t2.getRow()) + abs(t1.getCol()-t2.getCol());
}

void a_star(Board board) {
    priority_queue<Node_State> openList;
    unordered_map<string, bool> closedList;

    Tile player = board.getPlayerTile();
    std::list<Tile> targets = getTargets(board);

    while (!targets.empty()) {
        Tile goal;
        int closest = INT_MAX;
        for (const auto& target : targets) {
            int dist = distance(player, target);
            if (dist < closest) {
                // find the closest target
                closest = dist;
                goal = target;
            }
        }

        //start searching
        Node_State startNode(board, nullptr, 0, distance(player, goal));
        openList.push(startNode);

        while (!openList.empty()) {
            Node_State currentNode = openList.top();
            openList.pop();

            Board currentBoard = currentNode.getBoard();
            string boardKey = stringBoard(currentBoard);

            if (closedList[boardKey]) continue;
            closedList[boardKey] = true;

            // did player get rid of a target?
            if (currentBoard.getPlayerTile().getCol() == goal.getCol() && currentBoard.getPlayerTile().getRow() == goal.getRow()) {
                targets.remove(goal);
                player = currentBoard.getPlayerTile();

                break;
            }

            // if not, keep looking
            list<Board> nextStates = get_next_states(currentBoard);
            for (auto& nextBoard : nextStates) {
                string nextKey = stringBoard(nextBoard);
                if (!closedList[nextKey]) {
                    int g = currentNode.getG() + 1;
                    int h = distance(nextBoard.getPlayerTile(), goal);
                    Node_State nextNode(nextBoard, &currentNode, g, h);
                    openList.push(nextNode);
                }
            }
        }

        if (targets.empty()) {
            cout << "\033[38;5;226mYOU WIN!\033[0m\n";
            return;
        }
    }
}

#endif