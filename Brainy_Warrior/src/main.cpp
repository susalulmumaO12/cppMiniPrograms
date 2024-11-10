#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "./structure/tile.h"
#include "./structure/board.h"
#include "./logic/helper_functions.h"

using json = nlohmann::json;

using namespace std;

void printTile(int value) {
    switch (value) {
        case 0: cout << "\033[46m0\033[0m "; break;  // 0:sea
        case 1: cout << "\033[42m1\033[0m "; break;  // 1:land
        case 4: cout << "\033[103m4\033[0m "; break;  // 4:wall
        case 5: cout << "\033[35m5\033[0m "; break;  // 5:target
        case 6: cout << "\033[48;5;226m*\033[0m "; break;  // 6:star
        case 9: cout << "\033[31m9\033[0m "; break;  // 9:player
        case -1: cout << "\033[41mX\033[0m "; break;  // 9:player
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
    cout << "Enter a level number between 1 and 15: ";
    cin >> levelChoice;

    // is level number valid?
    if (levelChoice < 1 || levelChoice > 15) {
        cerr << "Invalid level number!" << endl;
        return 1;
    }

    // construct a string with level name "level i"
    string levelName = "level " + to_string(levelChoice);

    // does level exist?
    if (j.find(levelName) == j.end()) {
        cerr << "Level " << levelChoice << " not found!" << endl;
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

    bool win = false;
    while(!win){
    Tile& player = board.getPlayerTile();
    win = board.win();

    if(player.getValue() == -1){
        cout<<"\033[31mGAME OVER! You drowned...\033[0m\n";
        return 0;
    }

    if(win){
        cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
        return 0;
    }
        char m; cin>>m;
        board = move(board, player, m);
        cout << "Player position: (" << player.getRow() << ", " << player.getCol() << ")" << board.getTile(player.getRow(), player.getCol()).getValue() << endl;
        printBoard(board);
    }

    return 0;
}
