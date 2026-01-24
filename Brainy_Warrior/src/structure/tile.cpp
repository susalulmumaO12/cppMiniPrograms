#include "tile.h"

Tile::Tile(int row, int col, int value) {
    this->row = row;
    this->col = col;
    this->value = value;
    //0:sea, 1:land, 4:wall, 5:target, 6:star, 9:player
}

int Tile::getValue() const {
    return value;
}

void Tile::setValue(int value) {
    this->value = value;
}

int Tile::getRow() const {
    return row;
}

void Tile::setRow(int row) {
    this->row = row;
}

int Tile::getCol() const {
    return col;
}

void Tile::setCol(int col) {
    this->col = col;
}
