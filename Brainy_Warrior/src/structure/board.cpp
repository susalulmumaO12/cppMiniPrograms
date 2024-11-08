#include "board.h"
#include "tile.h"

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

const Tile& Board::getTile(int i, int j) const {
    return board[i][j];
}
