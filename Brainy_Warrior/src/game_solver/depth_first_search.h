#include<iostream>
#include<list>
#include<stack>
#include"../structure/board.h"
#include"../structure/tile.h"
#include"../include/main.h"


void dfs(Board board){

    stack<Board> s;
    s.push(board);

    bool win = false;
    while(!win || !s.empty()){

        Board current = s.top();
        win = current.win();
        Tile& player = board.getPlayerTile();


        printBoard(current);
        s.pop();

        list<Board> states = get_next_states(current, player);
        for(auto state:states){
            s.push(state);
        }

        if(win){
            cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
            exit(0);
        }
    }
}