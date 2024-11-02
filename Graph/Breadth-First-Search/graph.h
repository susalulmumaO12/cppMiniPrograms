/* 
Breadth-first search is one of the simplest algorithms for searching a graph and
the archetype for many important graph algorithms.

The following graph class is implemented specifically for Breadth-First Search algorithm, using adjacency lists to represent a graph G = (V, E).
 */
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <list>

class Graph {
private:
    int V; // vertices
    std::list<int>* EL; // edges list

public:
    Graph(int V); // constructor
    void insertEdge(int src, int des); // add a new edge
    int getVertices();
    std::list<int>* getEL();
    ~Graph(); // destructor
};

#endif
