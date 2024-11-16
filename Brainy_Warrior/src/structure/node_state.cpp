#include<iostream>
#include"Node_State.h"
#include"board.cpp"

Node_State::Node_State(Board board, Node_State* parent, int g, int h){
	this->board=board;
	this->parent=parent;
	this->g=g;
	this->h=h;
	this->f=g+h;
}

Board Node_State::getBoard(){
	return board;
}

Node_State* Node_State::getParent(){
	return parent;
}


int Node_State::getG() const{ 
	return g; 
}

int Node_State::getH() const{
	return h;
}

int Node_State::getF() const{
	return f;
}