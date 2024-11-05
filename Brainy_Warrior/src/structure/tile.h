#ifndef TILE_H
#define TILE_H

#include<iostream>

class Tile{

	private:
		int row;
		int col;
		int value;
	public:
		Tile(int row, int col, int value);
		int getValue();
		void setValue(int value);
		int getRow();
		void setRow(int row);
		int getCol();
		void setCol(int col);

}

#endif //TILE_H
