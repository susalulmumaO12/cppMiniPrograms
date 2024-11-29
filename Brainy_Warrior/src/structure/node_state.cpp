#include<iostream>
#include"node_state.h"
#include"board.h"

//Constructor
Node_State::Node_State(Board board, Node_State* parent, int g, int h)
    : board(board), parent(parent), g(g), h(h) {
    f = g + h;
}

//Deep Copy Constructor
Node_State::Node_State(const Node_State& other)
    : board(other.board), g(other.g), h(other.h), f(other.f) {
    // Copy the parent node if it exists, otherwise set it to nullptr
    if (other.parent != nullptr) {
        parent = new Node_State(*other.parent);
    } else {
        parent = nullptr;
    }
}

Board& Node_State::getBoard(){
	return board;
}

Node_State* Node_State::getParent(){
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