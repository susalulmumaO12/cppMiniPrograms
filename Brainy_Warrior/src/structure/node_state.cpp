#include<iostream>
#include"Node_State.h"
#include"board.cpp"

Node_State::Node_State(Board board, Node_State parent){
	this->board=board;
	this->parent=parent;
}

Board Node_State::getBoard(){
	return board;
}

Node_State Node_State::getParent(){
	return parent;
}
