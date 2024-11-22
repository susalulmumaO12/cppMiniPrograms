#ifndef NODE_STATE_H
#define NODE_STATE_H

#include<iostream>
#include"board.h"

class Node_State {
private:
    Board board;
    Node_State* parent;
	//cost from start to current
    int g;
	//heuristic
    int h;
	//total cost; f = g + h
    int f;

public:
    Node_State(Board board, Node_State* parent, int g, int h);

    Board& getBoard();
    Node_State* getParent();
    int getG();
    int getH();
    int getF();

	//defining < operator
    bool operator<(const Node_State& other) const {
        return f < other.f;
    }
};

#endif
