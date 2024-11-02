#include<iostream>
#include<queue>
#include"graph.h"

using namespace std;

void bfs(Graph g, int src){
    int V = g.getVertices();
    list<int>* el = g.getEL();
    bool* visited = new bool[V+1];

    for(int i=0; i<=V; i++){
         visited[i] = false;
    }

    //queue to store vertices and apply FIFO
    queue<int> u;
    
    //flag the given start point as visited
    visited[src] = true;
    u.push(src);

    while(!u.empty()){
        int s = u.front();
        cout<<"pointer at "<<s<<endl;
        u.pop();

        for(auto edge:el[s]){
            if(!visited[edge]){
                visited[edge] = true;
                u.push(edge);
            }
        }
    }

    delete[] visited;
} //bfs

int main(){

    /* 
        1------2------3
        |  __/        |
        4 /           5
        |
        6-----7
        |
        8
     */

    printf("        1------2------3\n        |  __/        |\n        4 /           5\n        |\n        6-----7\n        |\n        8\n");
    Graph graph(8);
    graph.insertEdge(1, 2);
    graph.insertEdge(2, 3);
    graph.insertEdge(2, 4);
    graph.insertEdge(1, 4);
    graph.insertEdge(3, 5);
    graph.insertEdge(4, 6);
    graph.insertEdge(6, 7);
    graph.insertEdge(6, 8);

    bfs(graph, 1);

    return 0;
}

//TODO: fix "Segmentation fault (core dumped)"
//TODO: support graphical representaion for BFS