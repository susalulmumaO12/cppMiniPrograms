#include "board.h"
#include<iostream>
#include "tile.h"

using namespace std;

Board::Board(int n, int m) {
    this->n = n;
    this->m = m;

    // Initialize 2d board
    board = std::vector<std::vector<Tile>>(n, std::vector<Tile>(m));

    // Initialize tiles
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            board[i][j] = Tile(i, j, 0);
        }
    }
}

int Board::getN() {
    return n;
}

int Board::getM() {
    return m;
}

Tile& Board::getTile(int i, int j) {
    return board[i][j];
}

Tile& Board::getPlayerTile() {
    for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if(board[i][j].getValue()== 9 || board[i][j].getValue()== -1){
                    //cout<<"DBG BOARD "<<board[i][j].getValue()<<endl;
                    return board[i][j];
                }
            }
        }
}

list<Tile&> getTargets(){
    list<Tile&> targets;
    for (int i = 0; i < board.getN(); i++) {
        for (int j = 0; j < board.getM(); j++) {
            if (board.getTile(i, j).getValue() == 5) {
               targets.push_back(board.getTile(i, j));
            }
        }
    }

    return target;
}

bool Board::win() {
    bool isWin = true;
    for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if(board[i][j].getValue() == 5){
                    isWin = false;
                }
            }
        }

        return isWin;
}

void Board::setTile(Tile& tile, int value) {

    tile.setValue(value);
}
