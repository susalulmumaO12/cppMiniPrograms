#ifndef MOVES_H
#define MOVES_H

#include<iostream>
#include <string>
#include<list>
#include"../structure/board.h"
#include"../structure/tile.h"
#include"../include/main.h"

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

 bool canMoveUp(Board b){

    Tile tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();

    if(i-1<0) return false;

    if(b.getTile(i-1, j).getValue() != 4){
        return true;
    }
    return false;
}

bool canMoveDown(Board b){

    Tile tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();
    int n=b.getN();

    if(i+1>=n) return false;

    if(b.getTile(i+1, j).getValue() != 4){
        return true;
    }
    return false;
}

bool canMoveLeft(Board b){

    Tile tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();

    if(j-1<0) return false;

    if(b.getTile(i, j-1).getValue() != 4){
        return true;
    }
    return false;
}

bool canMoveRight(Board b){

    Tile tile = b.getPlayerTile();
    int i=tile.getRow();
    int j=tile.getCol();
    int m=b.getM();

    if(j+1>=m) return false;

    if(b.getTile(i, j+1).getValue() != 4){
        return true;
    }
    return false;
}


Board moveUp(Board& b){

    Tile& tile = b.getPlayerTile();
    bool valid = canMoveUp(b);
    if(!valid){
        cout<<"\033[31mINVALID MOVE!\033[0m\n";
        return b;
    }

    int i=tile.getRow();
    int j=tile.getCol();
    
    int next = b.getTile(i-1, j).getValue();

    if(next == 0){
        // if upmost tile is 0: set player value to -1
        b.getTile(i, j).setValue(-1);
        return b;
    } else if(next == 1){
        swapTiles(b, tile, b.getTile(i-1, j));
        return b;
    } else if(next == 5 || next == 6){
        b.getTile(i-1, j).setValue(1);
        swapTiles(b, tile, b.getTile(i-1, j));
        return b;
    }

    return b;
}

Board moveDown(Board& b){

    Tile& tile = b.getPlayerTile();
    bool valid = canMoveDown(b);
    if(!valid){
        cout<<"\033[31mINVALID MOVE!\033[0m\n";
        return b;
    }

    int i=tile.getRow();
    int j=tile.getCol();

    int next = b.getTile(i+1, j).getValue();

    if(next == 0){
        // if downmost tile is 0: set player value to -1
        tile.setValue(-1);
        return b;
    } else if(next == 1){
        swapTiles(b, tile, b.getTile(i+1, j));
        return b;
    } else if(next == 5 || next == 6){
        b.getTile(i+1, j).setValue(1);
        swapTiles(b, tile, b.getTile(i+1, j));
        return b;
    }
    
    return b;
}

Board moveRight(Board& b){

    Tile& tile = b.getPlayerTile();
    bool valid = canMoveRight(b);
    if(!valid){
        cout<<"\033[31mINVALID MOVE!\033[0m\n";
        return b;
    }
    int i=tile.getRow();
    int j=tile.getCol();

    int next = b.getTile(i, j+1).getValue();

    if(next == 0){
		// if rightmost tile is 0: set player value to -1
        b.getTile(i, j).setValue(-1);
        return b;
    } else if(next == 1){
        swapTiles(b, tile, b.getTile(i, j+1));
        return b;
    } else if(next == 5 || next == 6){
        b.getTile(i, j+1).setValue(1);
        swapTiles(b, tile, b.getTile(i, j+1));
        return b;
    }

    return b;
}

Board moveLeft(Board& b){

    Tile& tile = b.getPlayerTile();
    bool valid = canMoveLeft(b);
    if(!valid){
        cout<<"\033[31mINVALID MOVE!\033[0m\n";
        return b;
    }
    int i=tile.getRow();
    int j=tile.getCol();
        
    int next = b.getTile(i, j-1).getValue();

    if(next == 0){
        // if leftmost tile is 0: set player value to -1
        b.getTile(i, j).setValue(-1);
        return b;
    } else if(next == 1){
        swapTiles(b, tile, b.getTile(i, j-1));
        return b;
    } else if(next == 5 || next == 6){
        b.getTile(i, j-1).setValue(1);
        swapTiles(b, tile, b.getTile(i, j-1));
        return b;
    }
    
    return b;
}

Board slideRight(Board& b){

    if(!canMoveRight(b)){
        cout<<"\033[44mInvalid move!\033[0m\n";
        return b;
    }
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
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(i, k-1));
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(i, k).setValue(1);
            swapTiles(b, tile, b.getTile(i, k));
            return b;
        }
    }
    return b;
}

Board slideLeft(Board& b){

    if(!canMoveLeft(b)){
        cout<<"\033[44mInvalid move!\033[0m\n";
        return b;
    }

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
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(i, k+1));
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(i, k).setValue(1);
            swapTiles(b, tile, b.getTile(i, k));
            return b;
        }
    }
    return b;
}

Board slideUp(Board& b){
    if(!canMoveUp(b)){
        cout<<"\033[44mInvalid move!\033[0m\n";
        return b;
    }
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
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(k+1, j));
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(k, j).setValue(1);
            swapTiles(b, tile, b.getTile(k, j));
            return b;
        }
    }
    return b;
}

Board slideDown(Board& b){
    if(!canMoveDown(b)){
        cout<<"\033[44mInvalid move!\033[0m\n";
        return b;
    }
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
        } else if(next == 4){
            swapTiles(b, tile, b.getTile(k-1, j));
            return b;
        } else if(next == 5 || next == 6){
            b.getTile(k, j).setValue(1);
            swapTiles(b, tile, b.getTile(k, j));
            return b;
        }
    }
    return b;
}

Board move(Board& b, char m){
    switch(tolower(m)){
        case 'i':
        case 'w':
            return !SLIDE? moveUp(b) : slideUp(b);
            break;
        case 'k':
        case 's':
            return !SLIDE? moveDown(b) : slideDown(b);
            break;
        case 'j':
        case 'a':
            return !SLIDE? moveLeft(b) : slideLeft(b);
            break;
        case 'l':
        case 'd':
            return !SLIDE? moveRight(b) : slideRight(b);
            break;
        default:
            cout<<"invalid move char";
            return b;
            break;
    }
}
#endif