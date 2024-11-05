#include<iostream>
#include"graph.h"
#include<vector>
#include"tile.h"

using namespace std;

class Board{

	int n, m;
	vector<vector<int>> board(n, vector<int> (m, 0));
	//a cell is filled with a number: 0:sea, 1:land, 4:wall, 5:target, 9:player
	
	Board(int n, int m){
		this->n=n;
		this->m=m;

	}

	void addCell(int i, int j, int type){
		board[i][j] = type;
		graph.addVertex(type);
	}

	bool canMoveUp(int i, int j){
		
		if(i-1<0) return false;

		if(board[i-1][j] != 4){
			return true;
		} else return false;
	}
	
	bool canMoveDown(int i, int j){
		
		if(i+1>=n) return false;

		if(board[i+1][j] != 4){
			return true;
		} else return false;
	}

	bool canMoveLeft(int i, int j){
		
		if(j-1<0) return false;

		if(board[i][j-1] != 4){
			return true;
		} else return false;
	}

	bool canMoveRight(int i, int j){
		
		if(j+1>=m) return false;

		if(board[i][j+1] != 4){
			return true;
		} else return false;
	}
}
