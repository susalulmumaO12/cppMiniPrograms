#include<iostream>
#include<vector>

using namespace std;

class Tile{
	private:
		int row;
		int col;
		int value;

	public:
		Tile(int row, int col, int value){
			this->row=row;
			this->col=col;
			this-value=value;
		}

		int getValue(){
			return value;
		}

		void setValue(int value){
			this->value=value;
		}

		int getRow(){
			return row;
		}

		void setRow(int row){
			this->row=row;
		}

		int getCol(){
			return col;
		}

		void setCol(int col){
			this->col=col;
		}

}
