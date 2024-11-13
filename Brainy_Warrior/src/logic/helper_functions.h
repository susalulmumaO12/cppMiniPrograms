#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include<iostream>
//#include"../structure/node_state.h"
#include<list>
#include"../structure/board.h"
#include"../structure/tile.h"
#include"../include/main.h"
#include"moves.h"

using namespace std;

list<Board> get_next_states(Board board){
    list<Board> states;
    //computer must check for valid moves
    //a move is valid when it doesn't end the game in losing state
    //canMove- functions check for valid moves, move- functions check for sea and walls for player
    //TODO: remove unneseccary redundant logic
    
    if(canMoveUp(board)){
        Board up = board;
        up = moveUp(up);
        if(up.getPlayerTile().getValue() != -1){

            states.push_back(up);
            cout<<"UP\n";
            printBoard(up);

            if(up.win()){
            return states;
            }
        }
    }

    if(canMoveDown(board)){
        Board down = board;
        down = moveDown(down);
        if(down.getPlayerTile().getValue() != -1){
        states.push_back(down);
        cout<<"DOWN\n";
        printBoard(down);

        if(down.win()){
            return states;
            }
        }
    }

    if(canMoveLeft(board)){
        Board left = board;
        left = moveLeft(left);
        if(left.getPlayerTile().getValue() != -1){
        states.push_back(left);
        cout<<"LEFT\n";
        printBoard(left);

        if(left.win()){
            return states;
            }
        }
    }

    if(canMoveRight(board)){
        Board right = board;
        right = moveRight(right);
        if(right.getPlayerTile().getValue() != -1){
        states.push_back(right);
        cout<<"RIGHT\n";
        printBoard(right);

            if(right.win()){
                return states;
            }
        }
    }

    return states;
}


bool isEqual(Board b1, Board b2){
    
    int n = b1.getN();
    int m = b2.getM();

    for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if(b1.getTile(i, j).getValue() != b2.getTile(i, j).getValue()){
                    return false;
                }
            }
    }

    return true;
}



#endif