#include<iostream>
#include<list>
#include<queue>
#include"../structure/board.h"
#include"../structure/tile.h"
#include"../include/main.h"

void bfs(Board board){

    queue<Board> q;
    q.push(board);

    bool win = false;
    while(!win || !q.empty()){

        Board current = q.front();
        win = current.win();
        Tile& player = board.getPlayerTile();


        printBoard(current);
        q.pop();

        list<Board> states = get_next_states(current, player);
        for(auto state:states){
            q.push(state);
        }

        if(win){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            exit(0);
        }
    }
}