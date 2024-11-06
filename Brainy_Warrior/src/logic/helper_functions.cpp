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

Board moveRight(Board b, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();
	int m=b.getM();

	for(int k=j+1; k<m; k++)
	{
		int next = b.board[i][k];

		if(next == 0){
			b.board[i][j]=1;
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.board[i][j]=1;
			b.board[i][k-1]=9;
			tile.setCol(k-1);
		} else if(next == 5){
			b.board[i][j]=1;
			b.board[i][k]=9;
			tile.setCol(k);
		} else if(next == 6){
			b.board[i][j]=1;
			b.board[i][k]=9;
			tile.setCol(k);
		}
	}
	return b;
}

Board moveLeft(Board b, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();
	int m=b.getM();

	for(int k=j-1; k>=0; k--)
	{
		int next = b.board[i][k];

		if(next == 0){
			b.board[i][j]=1;
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.board[i][j]=1;
			b.board[i][k+1]=9;
			tile.setCol(k+1);
		} else if(next == 5){
			b.board[i][j]=1;
			b.board[i][k]=9;
			tile.setCol(k);
		} else if(next == 6){
			b.board[i][j]=1;
			b.board[i][k]=9;
			tile.setCol(k);
		}
	}
	return b;
}

Board moveUp(Board b, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();
	int n=b.getN();

	for(int k=i-1; k>=0; k--)
	{
		int next = b.board[k][j];

		if(next == 0){
			b.board[i][j]=1;
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.board[i][j]=1;
			b.board[k+1][j]=9;
			tile.setRow(k+1);
		} else if(next == 5){
			b.board[i][j]=1;
			b.board[k][j]=9;
			tile.setRow(k);
		} else if(next == 6){
			b.board[i][j]=1;
			b.board[k][j]=9;
			tile.setRow(k);
		}
	}
	return b;
}

Board moveDown(Board b, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();
	int n=b.getN();

	for(int k=i+1; k<n; k++)
	{
		int next = b.board[k][j];

		if(next == 0){
			b.board[i][j]=1;
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.board[i][j]=1;
			b.board[k-1][j]=9;
			tile.setRow(k-1);
		} else if(next == 5){
			b.board[i][j]=1;
			b.board[k][j]=9;
			tile.setRow(k);
		} else if(next == 6){
			b.board[i][j]=1;
			b.board[k][j]=9;
			tile.setRow(k);
		}
	}
	return b;
}