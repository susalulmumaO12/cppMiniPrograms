#ifndef TILE_H
#define TILE_H

class Tile {
private:
    int row, col;
    int value;

public:
    // Default constructor with default values
    Tile(int row = 0, int col = 0, int value = 0);  

    int getValue();
    void setValue(int value);

    int getRow();
    void setRow(int row);

    int getCol();
    void setCol(int col);
};

#endif //TILE_H