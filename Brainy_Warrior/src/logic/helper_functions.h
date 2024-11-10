#include<iostream>
//#include"../structure/node_state.h"
#include<list>
#include"../structure/board.h"
#include"../structure/tile.h"

using namespace std;

void swapTiles(Board& b, Tile& tile1, Tile& tile2) {
    int row1 = tile1.getRow();
    int col1 = tile1.getCol();
    int row2 = tile2.getRow();
    int col2 = tile2.getCol();

    int value1 = b.getTile(row1, col1).getValue();
    int value2 = b.getTile(row2, col2).getValue();

    b.setTile(b.getTile(row1, col1), value2);
    b.setTile(b.getTile(row2, col2), value1);
}

bool canMoveUp(Board b, Tile tile){
    int i=tile.getRow();
    int j=tile.getCol();

    if(i-1<0) return false;

    if(b.getTile(i-1, j).getValue() != 4){
        return true;
    }
    return false;
}

bool canMoveDown(Board b, Tile tile){
    int i=tile.getRow();
    int j=tile.getCol();
    int n=b.getN();

    if(i+1>=n) return false;

    if(b.getTile(i+1, j).getValue() != 4){
        return true;
    }
    return false;
}

bool canMoveLeft(Board b, Tile tile){
    int i=tile.getRow();
    int j=tile.getCol();

    if(j-1<0) return false;

    if(b.getTile(i, j-1).getValue() != 4){
        return true;
    }
    return false;
}

bool canMoveRight(Board b, Tile tile){
    int i=tile.getRow();
    int j=tile.getCol();
    int m=b.getM();

    if(j+1>=m) return false;

    if(b.getTile(i, j+1).getValue() != 4){
        return true;
    }
    return false;
}

Board moveRight(Board& b, Tile& tile){
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

Board moveLeft(Board& b, Tile& tile){
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

Board moveUp(Board& b, Tile& tile){
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

Board moveDown(Board& b, Tile& tile){
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

    if(canMoveUp(board, tile)){
        Board up = board;
        up = moveUp(board, tile);
        states.push_back(up);
    }

    if(canMoveDown(board, tile)){
        Board down = board;
        down = moveDown(board, tile);
        states.push_back(down);
    }

    if(canMoveLeft(board, tile)){
        Board left = board;
        left = moveLeft(board, tile);
        states.push_back(left);
    }

    if(canMoveRight(board, tile)){
        Board right = board;
        right = moveRight(board, tile);
        states.push_back(right);
    }

    return states;
}

Board move(Board& b, Tile& t, char m){
    switch(m){
        case 'w':
            return moveUp(b, t);
        case 's':
            return moveDown(b, t);
        case 'a':
            return moveLeft(b, t);
        case 'd':
            return moveRight(b, t);
        default:
            cout<<"invalid move";
            return b;
    }
}