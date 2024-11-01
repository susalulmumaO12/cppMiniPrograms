/* 
Breadth-first search is one of the simplest algorithms for searching a graph and
the archetype for many important graph algorithms.

The following graph class is implemented specifically for Breadth-First Search algorithm, using adjacency lists to represent a graph G = (V, E).
 */
#include<iostream>
#include<list>

using namespace std;

class Graph
{
	private:
	int V;
	list<int>* EL; //edges list
	bool* visited;

	public:
	Graph(int V){ //constructor
		this->V = V;
		this->EL = new list<int>[V];
	}

	void insertEdge(int src, int des){
		//add an undirected edge
		EL[src].push_back(des);
		EL[des].push_back(src);
	}

	~Graph(){ //destructor
    delete[] this->EL;
    delete[] this->visited;
	}
};