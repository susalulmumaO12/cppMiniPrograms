#ifndef GRAPH_H
#define GRAPH_H

#include<iostream>
#include<list>

class Graph //weighted undirected graph
{
	private:
	int V; //vertices
	std::list* <int> EL; //edges list
	bool* visited;

	public:
	Graph(int V); //constructor
	void insertEdge(int src, int des); //add a new edge
	~Graph(); //destructor
};
#endif