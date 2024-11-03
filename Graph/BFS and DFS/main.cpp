#include<iostream>
#include<queue>
#include<stack>
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

void dfs(Graph g, int src){ //iterative dfs
    int V = g.getVertices();
    list<int>* el = g.getEL();
    bool* visited = new bool[V+1];

    for(int i=0; i<=V; i++){
         visited[i] = false;
    }

    stack<int> u;

    //flag given start point as visited
    visited[src] = true;
    u.push(src);

    while(!u.empty()){
        int s = u.top();
        cout<<"pointer at "<<s<<endl;
        u.pop();

        for(auto edge:el[s]){
            if(!visited[edge]){
                visited[edge] = true;
                u.push(edge);
            }
        }
    }
} //dfs

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

    int input, start;
    bool correct = false;

    cout<<"Choose traversal: 1) BFS, 2) DFS\n";
    cin>>input;
    cout<<"Choose starting point:\n";
    cin>>start;


    if(start>8)
    {
        while(!correct){
            cout<<"Invalid starting point, must be between 1-8:\n";
            cin>>start;
            if(start<=8) correct = true;
        }
    }
    
    input==1? bfs(graph, start) : dfs(graph, start);

    return 0;
}

//TODO: fix "Segmentation fault (core dumped)"
//TODO: support graphical representaion for BFS