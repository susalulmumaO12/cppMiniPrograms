#ifndef NODE_STATE_H
#define NODE_STATE_H

#include<iostream>
#include"board.cpp"

class Node_State{
	private:
		Board board;
		Node_State parent;
	
	public:
		Node_State(Board board, Node_State parent);
		Board getBoard();
		Node_State getParent();

};

#endif
