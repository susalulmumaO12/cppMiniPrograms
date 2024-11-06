#include<iostream>
#include"graph.h"
#include<vector>
#include"tile.h"

using namespace std;

class Board{

	int n, m;
	vector<vector<Tile>> board(n, vector<Tile> (m, 0));
	//a cell is filled with a number: 0:sea, 1:land, 4:wall, 5:target, 6:star, 9:player
	
	Board(int n, int m){
		this->n=n;
		this->m=m;
	}

	void addCell(int i, int j, int type){
		board[i][j] = type;
		graph.addVertex(type);
	}

	int getN(){
		return n;
	}

	int getM(){
		return m;
	}

}
