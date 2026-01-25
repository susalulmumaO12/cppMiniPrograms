#include "board.h"
#include <iostream>
#include <list>
#include "tile.h"

using namespace std;

Board::Board(int n, int m) {
    this->n = n;
    this->m = m;

    // Initialize 2d board
    board = vector<vector<Tile>>(n, vector<Tile>(m));

    // Initialize tiles
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            board[i][j] = Tile(i, j, 0);
        }
    }
}

Board::Board(const Board& other) {

    n = other.n;
    m = other.m;
    wizardUsed = other.wizardUsed;
    wizardTiles = other.wizardTiles;
    currentWizardIndex = other.currentWizardIndex;

    // Initialize 2d board
    board = vector<vector<Tile>>(n, vector<Tile>(m));

    // Initialize tiles
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            board[i][j] = other.board[i][j];
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
            if (board[i][j].getValue() == 9 || board[i][j].getValue() == -1) {
                return board[i][j];
            }
        }
    }
    // Return a dummy tile if not found (should not happen with valid levels)
    static Tile dummy(-1, -1, -1);
    return dummy;
}

list<Tile> Board::getTargetTiles(){
    list <Tile> targets;
    for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (board[i][j].getValue() == 5) {
                    targets.push_back(board[i][j]);
                }
            }
        }
        return targets;
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

bool Board::isWizardUsed() const { return wizardUsed; }
void Board::setWizardUsed(bool used) { wizardUsed = used; }
const std::vector<Tile>& Board::getWizardTiles() const { return wizardTiles; }
void Board::setWizardTiles(const std::vector<Tile>& tiles) { wizardTiles = tiles; }
int Board::getCurrentWizardIndex() const { return currentWizardIndex; }
void Board::setCurrentWizardIndex(int index) { currentWizardIndex = index; }
