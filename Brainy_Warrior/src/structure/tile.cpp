#include"TILE_H"

Tile::Tile(int row, int col, int value){
	this->row=row;
	this->col=col;
	this-value=value;
}

int Tile::getValue(){
	return value;
}

void Tile::setValue(int value){
	this->value=value;
}

int Tile::getRow(){
	return row;
}

void Tile::setRow(int row){
	this->row=row;
}

int Tile::getCol(){
	return col;
}

void Tile::setCol(int col){
	this->col=col;
}


