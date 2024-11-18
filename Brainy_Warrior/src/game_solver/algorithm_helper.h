#ifndef ALGORITHM_HELPER_H
#define ALGORITHM_HELPER_H

#define DEBUG false
#include<list>
#include"../structure/board.h"
#include"../structure/node_state.h"
#include"../logic/helper_functions.h"


using namespace std;

bool isVisited(list<Board> visitedStates, Board state){

    for(auto vState:visitedStates){

        if(isEqual(vState, state)){
            return true;
        }
    }

    return false;

}

string stringBoard(Board& b) {
    string boardHash;
    int n = b.getN();
    int m = b.getM();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            boardHash += to_string(b.getTile(i, j).getValue());
            boardHash += ',';
        }
    }
    return boardHash;
}


list<Tile> getTargets(Board board){
    list<Tile> targets;
    for (int i = 0; i < board.getN(); i++) {
        for (int j = 0; j < board.getM(); j++) {
            if (board.getTile(i, j).getValue() == 5) {
               targets.push_back(board.getTile(i, j));
            }
        }
    }

    return targets;
}

list<Node_State> get_next_cost_states(Node_State* board){
    list<Node_State> states;
    
    if(canMoveUp(board->getBoard())){ //cost of moving up: 1
        Board up = board->getBoard();
        up = moveUp(up);
        if(up.getPlayerTile().getValue() != -1){

            Node_State up_state(up, board, 1, 0);
            states.push_back(up_state);
            if(DEBUG){

            cout<<"UP\n";
            printBoard(up);
            }

            if(up.win()){
            return states;
            }
        }
    }

    if(canMoveDown(board->getBoard())){ //cost of moving down: 2
        Board down = board->getBoard();
        down = moveDown(down);
        if(down.getPlayerTile().getValue() != -1){


            Node_State down_state(down, board, 2, 0);
            states.push_back(down_state);
            if(DEBUG){
            cout<<"DOWN\n";
            printBoard(down);
        }

        if(down.win()){
            return states;
            }
        }
    }

    if(canMoveLeft(board->getBoard())){ //cost of moving left: 5
        Board left = board->getBoard();
        left = moveLeft(left);
        if(left.getPlayerTile().getValue() != -1){


            Node_State left_state(left, board, 5, 0);
        states.push_back(left_state);

        if(DEBUG){

        cout<<"LEFT\n";
        printBoard(left);
        }

        if(left.win()){
            return states;
            }
        }
    }

    if(canMoveRight(board->getBoard())){ //cost of moving right: 3
        Board right = board->getBoard();
        right = moveRight(right);
        if(right.getPlayerTile().getValue() != -1){


            Node_State right_state(right, board, 3, 0);
        states.push_back(right_state);
        if(DEBUG){

        cout<<"RIGHT\n";
        printBoard(right);
        }

            if(right.win()){
                return states;
            }
        }
    }

    return states;
}
#endif