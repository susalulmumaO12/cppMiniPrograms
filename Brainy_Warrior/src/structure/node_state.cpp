#include <iostream>
#include "node_state.h"
#include "board.h"

//Constructor
Node_State::Node_State(Board board, std::shared_ptr<Node_State> parent, int g, int h)
    : board(board), parent(parent), g(g), h(h) {
    f = g + h;
}

Board& Node_State::getBoard(){
	return board;
}

std::shared_ptr<Node_State> Node_State::getParent(){
	return parent;
}


int Node_State::getG(){ 
	return g; 
}

int Node_State::getH(){
	return h;
}

int Node_State::getF(){

	return f;
}
