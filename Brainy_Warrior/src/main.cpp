#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "./structure/tile.h"
#include "./structure/board.h"
#include "./logic/helper_functions.h"
#include "./logic/moves.h"
#include "./game_solver/breadth_first_search.h"
#include "./game_solver/depth_first_search.h"
#include "./game_solver/uniform_cost_search.h"
#include "./game_solver/A_star_search.h"
#include "./game_solver/hill_climbing.h"

using json = nlohmann::json;

using namespace std;

void printTile(int value) {
    switch (value) {
        case 0: cout << "\033[48;5;45m\033[38;5;75m 0 \033[0m\033[0m"; break;  // 0:sea
        case 1: cout << "\033[48;5;42m\033[38;5;36m 1 \033[0m\033[0m"; break;  // 1:land
        case 4: cout << "\033[48;5;226m\033[38;5;220m 4 \033[0m\033[0m"; break;  // 4:wall
        case 5: cout << "\033[48;5;96m\033[1;38;5;89m 5 \033[0m\033[0m"; break;  // 5:target
        case 6: cout << "\033[38;5;226m * \033[0m"; break;  // 6:star
        case 8: cout << "\033[48;5;200m\033[1;38;5;9m 8 \033[0m\033[0m"; break;  // 8:won player
        case 9: cout << "\033[48;5;216m\033[1;38;5;9m 9 \033[0m\033[0m"; break;  // 9:player
        case -1: cout << "\033[48;5;9m\033[1;38;5;216m X \033[0m\033[0m"; break;  // -1:dead player
        default: cout << value << " "; break;
    }
}

void printBoard(Board board){
    int n = board.getN();
    int m = board.getM();

    for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printTile(board.getTile(i, j).getValue());
            }
            cout << endl;
    }

    cout<<endl;
}

int main() {
    ifstream inputFile("../src/levels.json");
    if (!inputFile.is_open()) {
        cerr << "Error opening levels.json file!" << endl;
        return 1;
    }

    // create an empty structure (null)
    json j;
    inputFile >> j;

    // get user input for level number
    int levelChoice;
    cout<<"Enter a level number between 1 and 15: ";
    cin>>levelChoice;

    int playingOption;
    cout<<"Playing options: 1) User, 2) Computer: ";
    cin>>playingOption;

    // is level number valid?
    if (levelChoice < 1 || levelChoice > 15) {
        cerr<<"Invalid level number!"<<endl;
        return 1;
    }

    // construct a string with level name "level i"
    string levelName = "level " + to_string(levelChoice);

    // does level exist?
    if (j.find(levelName) == j.end()) {
        cerr<<"Level "<<levelChoice<<" not found!"<<endl;
        return 1;
    }

    // level data from json file
    auto& levelData = j[levelName];
    int n = levelData.size();
    int m = levelData[0].size();

    // create board from json data
    Board board(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            board.getTile(i, j).setValue(levelData[i][j]);
        }
    }

        

    cout << "Board for " << levelName << ":\n";
    printBoard(board);

    // USER PLAYING
    if(playingOption == 1){
int difficulty;
        cout<<"Choose difficulty level: \n\033[46m1) Easy (tile move style)\033[0m\n\033[41m2) Hard (slide style)\033[0m\n";
        cin>>difficulty;
        
        while(!board.win()){
            Tile& player = board.getPlayerTile();

            if(player.getValue() == -1){
                cout<<"\033[31mGAME OVER! You drowned...\033[0m\n";
                return 0;
            }

            char m; cin>>m;
            board = difficulty == 1? move(board, m): slide(board, m);
            cout << "Player position: (" << player.getRow() << ", " << player.getCol() << ")" << endl;
            printBoard(board);

            if(board.win()){
                cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
                return 0;
            }
        }

    } else if (playingOption == 2){

        int algorithm;
        cout<<"Choose algorithm: 1) BFS, 2) DFS, 3) UCS 4) Hill Climbing 5) A_star\n";
        cin>>algorithm;
        if(algorithm == 1) bfs(board);
        else if(algorithm == 2) dfs(board);
        else if(algorithm == 3) ucs(board);
        else if(algorithm == 4) hill_climbing(board);
        else if(algorithm == 5) a_star(board);
    }
    
    return 0;
}
