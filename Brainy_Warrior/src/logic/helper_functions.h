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


Board slideRight(Board& b){

    Tile& tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();
    int m=b.getM();

    for(int k=j+1; k<m; k++){
        int next = b.getTile(i, k).getValue();

        if(next == 0){
			// if rightmost tile is 0: set player value tp -1
            tile.setValue(-1);
            swapTiles(b, tile, b.getTile(i, j));
            return b;
        } else if(next == 1){
            continue;
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(i, k-1));
            tile.setCol(k-1);
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(i, k).setValue(1);
            swapTiles(b, tile, b.getTile(i, k));
            tile.setCol(k);
            return b;
        }
    }
    return b;
}

Board slideLeft(Board& b){

    Tile& tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();

    for(int k=j-1; k>=0; k--){
        int next = b.getTile(i, k).getValue();

        if(next == 0){
            // if leftmost tile is 0: set player value tp -1
            tile.setValue(-1);
			//cout<<"DBG new tile value is: "<<tile.getValue()<<endl;
            swapTiles(b, tile, b.getTile(i, j));
            return b;
        } else if(next == 1){
            continue;
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(i, k+1));
            tile.setCol(k+1);
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(i, k).setValue(1);
            swapTiles(b, tile, b.getTile(i, k));
            tile.setCol(k);
            return b;
        }
    }
    return b;
}

Board slideUp(Board& b){
    
    Tile& tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();

    for(int k=i-1; k>=0; k--){
        int next = b.getTile(k, j).getValue();

        if(next == 0){
            // if upmost tile is 0: set player value tp -1
            tile.setValue(-1);
            swapTiles(b, tile, b.getTile(i, j));
            return b;
        } else if(next == 1){
            continue;
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(k+1, j));
            tile.setRow(k+1);
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(k, j).setValue(1);
            swapTiles(b, tile, b.getTile(k, j));
            tile.setRow(k);
            return b;
        }
    }
    return b;
}

Board slideDown(Board& b){

    Tile& tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();
    int n=b.getN();

    for(int k=i+1; k<n; k++){
        int next = b.getTile(k, j).getValue();

        if(next == 0){
            // if downmost tile is 0: set player value tp -1
            tile.setValue(-1);
            swapTiles(b, tile, b.getTile(i, j));
            return b;
        } else if(next == 1){
            continue;
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(k-1, j));
            tile.setRow(k-1);
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(k, j).setValue(1);
            swapTiles(b, tile, b.getTile(k, j));
            tile.setRow(k);
            return b;
        }
    }
    return b;
}

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
        }
    }

    if(canMoveDown(board)){
        Board down = board;
        down = moveDown(down);
        if(down.getPlayerTile().getValue() != -1){
        states.push_back(down);
        cout<<"DOWN\n";
        printBoard(down);
        }
    }

    if(canMoveLeft(board)){
        Board left = board;
        left = moveLeft(left);
        if(left.getPlayerTile().getValue() != -1){
        states.push_back(left);
        cout<<"LEFT\n";
        printBoard(left);
        }
    }

    if(canMoveRight(board)){
        Board right = board;
        right = moveRight(right);
        if(right.getPlayerTile().getValue() != -1){
        states.push_back(right);
        cout<<"RIGHT\n";
        printBoard(right);
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