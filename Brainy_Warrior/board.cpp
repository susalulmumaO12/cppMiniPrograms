#include<iostream>

using namespace std;

class Board{

	int n, m;
	vector<vector<int>> board(n, vector<int> (m, 0));
	//a cell is filled with a number: 0:empty, 1:player, 4:wall, 5:target
	//
	
	Board(int n, int m){
		this->n=n;
		this->m=m;

	}

	void addCell(int i, int j, int type){
		board[i][j] = type;
	}
}
