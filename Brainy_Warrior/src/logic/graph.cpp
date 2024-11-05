#include "graph.h"
#include <iostream>
#include <list>

using namespace std;

Graph::Graph(int V) { // constructor
    this->V = V;
    this->EL = new list<int>[V+1];
}

void Graph::insertEdge(int src, int des) {
    // add an undirected edge
    EL[src].push_back(des);
    EL[des].push_back(src);
}

void Graph::insertVertex(int type){

}

int Graph::getVertices() {
    return V;
}

list<int>* Graph::getEL() {
    return EL;
}

Graph::~Graph() { // destructor
    delete[] this->EL;
}
