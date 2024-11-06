#include<iostream>
#include"../structure/Node_State.h"
#include<list>
#include"../structure/board.cpp"


using namespace std;

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
}


bool canMoveUp(Board board, Tile tile){
		
	int i=tile.getRow();
	int j=tile.getCol();

	if(i-1<0) return false;

	if(board[i-1][j] != 4){
		return true;
	} else return false;
}
	
bool canMoveDown(Board board, Tile tile){
		
	int i=tile.getRow();
	int j=tile.getCol();
	int n=board.getN();
	if(i+1>=n) return false;

	if(board[i+1][j] != 4){
		return true;
	} else return false;
}

bool canMoveLeft(Board board, Tile tile){
	
	int i=tile.getRow();
	int j=tile.getCol();

	if(j-1<0) return false;

	if(board[i][j-1] != 4){
		return true;
	} else return false;
}

bool canMoveRight(Board board, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();
	int m=board.getM();

	if(j+1>=m) return false;

	if(board[i][j+1] != 4){
		return true;
	} else return false;
}
