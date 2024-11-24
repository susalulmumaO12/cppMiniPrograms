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

// Function to remove a target from the vector
void removeTarget(vector<pair<int, Tile>>& targets, Tile& target) {
    targets.erase(remove_if(targets.begin(), targets.end(),
                             [&target](pair<int, Tile>& p) {
                                 return p.second.getCol() == target.getCol() &&
                                        p.second.getRow() == target.getRow();
                             }), 
                   targets.end());
}

void a_star(Board& board) {
    priority_queue<Node_State> openList;
    unordered_map<string, bool> closedList;

    Tile player = board.getPlayerTile();

    vector<pair<int ,Tile>> targets;
    for (int i = 0; i < board.getN(); ++i) {
        for (int j = 0; j < board.getM(); ++j) {
            if (board.getTile(i, j).getValue() == 5) {
                targets.emplace_back(distance(player, board.getTile(i,j)), board.getTile(i,j));
            }
        }
    } 

    while (!targets.empty()) {
        auto minTarget = std::min_element(targets.begin(), targets.end(),
                                           [](const pair<int, Tile>& a, const pair<int, Tile>& b) {
                                               return a.first < b.first;
                                           });
        Tile goal = minTarget->second;

        // Start searching
        Node_State startNode(board, nullptr, 0, distance(player, goal));
        openList.push(startNode);

        while (!openList.empty()) {
            Node_State currentNode = openList.top();
            openList.pop();

            Board currentBoard = currentNode.getBoard();
            string boardKey = stringBoard(currentBoard);

            if (closedList[boardKey]) continue;
            closedList[boardKey] = true;
            printBoard(currentBoard);

            // Did player get rid of a target?
            if (currentBoard.getPlayerTile().getCol() == goal.getCol() && currentBoard.getPlayerTile().getRow() == goal.getRow()) {
                removeTarget(targets, goal);
                player = currentBoard.getPlayerTile();

                //recalculate distances for remaining targets
                for (auto& target : targets) {
                    target.first = distance(player, target.second);
                }
                break;
            }

            // If not, keep looking
            list<Board> nextStates = get_next_states(currentBoard);
            for (auto& nextBoard : nextStates) {
                string nextKey = stringBoard(nextBoard);
                if (!closedList[nextKey]) {
                    int g = currentNode.getG() + 1;
                    cout<<"\ncurrent g and next g: "<<currentNode.getG()<<" "<<g<<endl;
                    int h = distance(nextBoard.getPlayerTile(), goal);
                    Node_State nextNode(nextBoard, &currentNode, g, h);
                    openList.push(nextNode);
                }
            }
        }
        //win state
        if (targets.empty()) {
            cout << "\033[38;5;226mYOU WIN!\033[0m\n";
            return;
        }
    }
}

#endif