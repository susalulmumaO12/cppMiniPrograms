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


Board slideRight(Board& b, Tile& tile){
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

Board slideLeft(Board& b, Tile& tile){
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

Board slideUp(Board& b, Tile& tile){
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

Board slideDown(Board& b, Tile& tile){
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

list<Board> get_next_states(Board board, Tile tile){
    list<Board> states;

    //computer must check for valid moves
    //a move is valid when it doesn't end the game in losing state
    //canMove- functions check for valid moves, move- functions check for sea and walls for player
    //TODO: remove unneseccary redundant logic
    
    if(canMoveUp(board, tile)){
        Board up = board;
        up = moveUp(board, tile);
        states.push_back(up);

        cout<<"UP\n";
        printBoard(up);
    }

    if(canMoveDown(board, tile)){
        Board down = board;
        down = moveDown(board, tile);
        states.push_back(down);

        cout<<"DOWN\n";
        printBoard(down);
    }

    if(canMoveLeft(board, tile)){
        Board left = board;
        left = moveLeft(board, tile);
        states.push_back(left);

        cout<<"LEFT\n";
        printBoard(left);
    }

    if(canMoveRight(board, tile)){
        Board right = board;
        right = moveRight(board, tile);
        states.push_back(right);

        cout<<"RIGHT\n";
        printBoard(right);
    }

    return states;
}


#endif