#include<iostream>
//#include"../structure/node_state.h"
#include<list>
#include"../structure/board.h"
#include"../structure/tile.h"

using namespace std;

bool canMoveUp(Board b, Tile tile){
		
	int i=tile.getRow();
	int j=tile.getCol();

	if(i-1<0) return false;

	if(b.getTile(i-1, j).getValue() != 4){
		return true;
	} else return false;

	return false;
}
	
bool canMoveDown(Board b, Tile tile){
		
	int i=tile.getRow();
	int j=tile.getCol();
	int n=b.getN();
	if(i+1>=n) return false;

	if(b.getTile(i+1, j).getValue() != 4){
		return true;
	} else return false;

	return false;
}

bool canMoveLeft(Board b, Tile tile){
	
	int i=tile.getRow();
	int j=tile.getCol();

	if(j-1<0) return false;

	if(b.getTile(i, j-1).getValue() != 4){
		return true;
	} else return false;

	return false;
}

bool canMoveRight(Board b, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();
	int m=b.getM();

	if(j+1>=m) return false;

	if(b.getTile(i, j+1).getValue() != 4){
		return true;
	} else return false;

	return false;
}

Board moveRight(Board b, Tile& tile){

	int i=tile.getRow();
	int j=tile.getCol();
	int m=b.getM();

	for(int k=j+1; k<m; k++)
	{
		int next = b.getTile(i, k).getValue();

		if(next == 0){
			b.setTile(b.getTile(i, j), 1);
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(i, k-1), 9);
			tile.setCol(k-1);
		} else if(next == 5){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(i, k), 9);
			tile.setCol(k);
		} else if(next == 6){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(i, k), 9);
			tile.setCol(k);
		}
	}
	return b;
}

Board moveLeft(Board b, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();

	for(int k=j-1; k>=0; k--)
	{
		int next = b.getTile(i, k).getValue();

		if(next == 0){
			b.setTile(b.getTile(i, j), 1);
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(i, k+1), 9);
			tile.setCol(k+1);
		} else if(next == 5){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(i, k), 9);
			tile.setCol(k);
		} else if(next == 6){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(i, k), 9);
			tile.setCol(k);
		}
	}
	return b;
}

Board moveUp(Board b, Tile tile){

	int i=tile.getRow();
	int j=tile.getCol();

	for(int k=i-1; k>=0; k--)
	{
		int next = b.getTile(k, j).getValue();

		if(next == 0){
			b.setTile(b.getTile(i, j), 1);
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(k+1, j), 9);
			tile.setRow(k+1);
		} else if(next == 5){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(k, j), 9);
			tile.setRow(k);
		} else if(next == 6){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(k, j), 9);
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
		int next = b.getTile(k, j).getValue();

		if(next == 0){
			b.setTile(b.getTile(i, j), 1);
			return b;
		} else if(next == 1){
			continue;
		} else if(next == 4){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(k-1, j), 9);
			tile.setRow(k-1);
		} else if(next == 5){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(k, j), 9);
			tile.setRow(k);
		} else if(next == 6){
			b.setTile(b.getTile(i, j), 1);
			b.setTile(b.getTile(k, j), 9);
			tile.setRow(k);
		}
	}
	return b;
}

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

	return states;
}


Board move(Board b, Tile& t, char m){
	switch(m){
		case 'w':
			return moveUp(b, t);
		break;

		case 's':
			return moveDown(b, t);
		break;

		case 'a':
			return moveLeft(b, t);
		break;

		case 'd':
			return moveRight(b, t);
		break;

		default: cout<<"invalid move"; return b;
	}
	return b;
}