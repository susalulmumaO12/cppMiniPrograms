#include "helper_functions.h"
#define DEBUG false

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <list>
#include <vector>
#include <cmath>
#include "node_state.h"
#include "board.h"
#include "tile.h"
#include "main.h"
#include "moves.h"

using json = nlohmann::json;

using namespace std;

Board set_game_board(std::string levelKey){
    ifstream inputFile("../src/levels.json");
    if (!inputFile.is_open()) {
        cerr << "Error opening levels.json file!" << endl;
        exit(0);
    }

    // create an empty structure (null)
    json j;
    inputFile >> j;

    if (j.find(levelKey) == j.end()) {
        cerr<<"Level "<<levelKey<<" not found!"<<endl;
        exit(0);
    }

    // level data from json file
    auto& levelData = j[levelKey]["layout"];
    int n = levelData.size();
    int m = levelData[0].size();

    // create board from json data
    Board board(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            board.getTile(i, j).setValue(levelData[i][j]);
        }
    }

    return board;
}

void printTile(int value) {
    switch (value) {
        case 0: cout << "\033[48;5;45m\033[38;5;75m 0 \033[0m\033[0m"; break;  // 0:sea
        case 1: cout << "\033[48;5;42m\033[38;5;36m 1 \033[0m\033[0m"; break;  // 1:land
        case 3: cout << "\033[48;5;22m\033[38;5;165m O \033[0m\033[0m"; break;  // 3:wizard tile
        case 4: cout << "\033[48;5;226m\033[38;5;220m 4 \033[0m\033[0m"; break;  // 4:wall
        case 5: cout << "\033[48;5;96m\033[1;38;5;89m 5 \033[0m\033[0m"; break;  // 5:target
        case 6: cout << "\033[38;5;226m * \033[0m"; break;                      // 6:star
        case 7: cout << "\033[48;5;165m\033[1;38;5;22m W \033[0m\033[0m"; break;  // 7:wizard
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

vector<pair<int, pair<int, pair<int, string>>>> fetchStats(string levelName) {
    json stats;
    vector<pair<int, pair<int, pair<int, string>>>> score_pairs;

    // Load existing stats
    ifstream inputFile("../stats.json");
    if (inputFile.is_open()) {
        inputFile >> stats;
        inputFile.close();
    }

    if (levelName != "All stats") { // TODO: this method is stupid, is it still???

        if (!stats["levels"].contains(levelName)) {
            cout << "No one played this level yet!\n";
        }

        for (auto player : stats["levels"][levelName].items()) {
            string playerName = player.key();
            int playerScore = player.value()["score"];
            int playerTries = player.value()["tries"];
            int playerWins = player.value()["wins"];

            score_pairs.push_back({playerScore, {playerTries, {playerWins, playerName}}});
        }

        sort(score_pairs.begin(), score_pairs.end());

    } else { // print all levels stats
        for (auto levelItem : stats["levels"].items()) {
            string levelName = levelItem.key();
            score_pairs.clear();
            
            for (auto player : levelItem.value().items()) {  
                string playerName = player.key();
                int playerScore = player.value()["score"];
                int playerTries = player.value()["tries"];
                int playerWins = player.value()["wins"];

                score_pairs.push_back({playerScore, {playerTries, {playerWins, playerName}}});
            }

            sort(score_pairs.begin(), score_pairs.end());
        }
    }

    return score_pairs;
}

void updateStats(const string& playerName, string levelName, bool win) {
    json stats;
    string filePath = "../stats.json";

    ifstream inputFile(filePath);
    if (inputFile.is_open()) {
        try {
            inputFile >> stats;
        } catch (json::parse_error& e) {
            cerr << "Error parsing stats.json. Initializing a new file." << endl;
            stats = json::object();
        }
        inputFile.close();
    } else {
        ofstream outputFile(filePath);
        if (outputFile.is_open()) {
            outputFile << "{}";
            outputFile.close();
        }
        stats = json::object();
    }

    if (!stats.contains("levels")) {
        stats["levels"] = json::object();
    }
    if (!stats["levels"].contains(levelName)) {
        stats["levels"][levelName] = json::object();
    }
    if (!stats["levels"][levelName].contains(playerName)) {
        stats["levels"][levelName][playerName] = {{"tries", 0}, {"wins", 0}, {"score", 0}};
    }

    stats["levels"][levelName][playerName]["tries"] = stats["levels"][levelName][playerName]["tries"].get<int>() + 1;

    if (win) {
        stats["levels"][levelName][playerName]["wins"] = stats["levels"][levelName][playerName]["wins"].get<int>() + 1;
    }

    if (!stats["levels"][levelName][playerName].contains("score") || 
        (SCORE < stats["levels"][levelName][playerName]["score"].get<int>() ||
        stats["levels"][levelName][playerName]["score"].get<int>() == 0)) {
        cout << "New best score for " << playerName << "! Score: " << SCORE << endl;
        stats["levels"][levelName][playerName]["score"] = SCORE;
    }

    //save the updated stats
    ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        outputFile << stats.dump(4);
        outputFile.close();
    } else {
        cerr << "Error: Unable to write to stats.json" << endl;
    }
}

dist getDistanceType() {
    int choice;

    // Displaying the menu with ANSI styling
    cout << "\033[1;37mSelect heuristic calculation:\033[0m\n";
    cout << "\033[48;5;124m\033[38;5;15m1) Manhattan\033[0m\033[0m ";
    cout << "\033[48;5;34m\033[38;5;15m2) Euclidean\033[0m\033[0m ";
    cout << "\033[48;5;208m\033[38;5;15m3) Minkowski (p=3)\033[0m\033[0m ";
    cout << "\033[48;5;27m\033[38;5;15m4) Chebyshev\033[0m\033[0m ";
    
    cin >> choice;

    // Returning the corresponding distance type
    switch (choice) {
        case 1:
            return man; // Manhattan
        case 2:
            return euc; // Euclidean
        case 3:
            return mink; // Minkowski
        case 4:
            return cheb; // Chebyshev
        default:
            std::cout << "\033[31mInvalid choice, defaulting to Manhattan.\033[0m\n";
            return man; // Default to Manhattan
    }
}

list<Node_State> get_next_cost_states(Node_State* board){
    list<Node_State> states;
    
    if(canMoveUp(board->getBoard())){ //cost of moving up: 1
        Board up(board->getBoard());
        up = slideUp(up);
        if(up.getPlayerTile().getValue() != -1){

            Node_State up_state(up, board, board->getG() + 1, 0);
            states.push_back(up_state);
            if(DEBUG){

            cout<<"UP\n";
            printBoard(up);
            }

            if(up.win()){
            return states;
            }
        }
    }

    if(canMoveDown(board->getBoard())){ //cost of moving down: 2
        Board down(board->getBoard());
        down = slideDown(down);
        if(down.getPlayerTile().getValue() != -1){


            Node_State down_state(down, board, board->getG() + 2, 0);
            states.push_back(down_state);
            if(DEBUG){
            cout<<"DOWN\n";
            printBoard(down);
        }

        if(down.win()){
            return states;
            }
        }
    }

    if(canMoveLeft(board->getBoard())){ //cost of moving left: 5
        Board left(board->getBoard());
        left = slideLeft(left);
        if(left.getPlayerTile().getValue() != -1){


            Node_State left_state(left, board, board->getG() + 5, 0);
            states.push_back(left_state);

        if(DEBUG){

        cout<<"LEFT\n";
        printBoard(left);
        }

        if(left.win()){
            return states;
            }
        }
    }

    if(canMoveRight(board->getBoard())){ //cost of moving right: 3
        Board right(board->getBoard());
        right = slideRight(right);
        if(right.getPlayerTile().getValue() != -1){


            Node_State right_state(right, board, board->getG() + 3, 0);
        states.push_back(right_state);
        if(DEBUG){

        cout<<"RIGHT\n";
        printBoard(right);
        }

            if(right.win()){
                return states;
            }
        }
    }

    return states;
}

int distance(Tile t1, Tile t2) {
    int minkowskiP = 3;
    // Extract coordinates as vectors for generalized calculations
    vector<int> v1 = {t1.getRow(), t1.getCol()};
    vector<int> v2 = {t2.getRow(), t2.getCol()};

    switch (distanceType) {
        case man:
            // Manhattan distance - we can only move in 4 directions
            return abs(t1.getRow() - t2.getRow()) + abs(t1.getCol() - t2.getCol());
        
        case euc:
            // Euclidean distance
            return static_cast<int>(sqrt((t1.getRow() - t2.getRow()) * (t1.getRow() - t2.getRow()) +
                                          (t1.getCol() - t2.getCol()) * (t1.getCol() - t2.getCol())));
        
        case mink: {
            // Minkowski distance (generalized form)
            double sum = 0.0;
            for (size_t i = 0; i < v1.size(); ++i) {
                sum += pow(abs(v1[i] - v2[i]), minkowskiP);
            }
            return static_cast<int>(pow(sum, 1.0 / minkowskiP));
        }

        case cheb: {
            
            return static_cast<int>(max((t2.getRow() - t1.getRow()), (t2.getCol() - t1.getCol())));
        }

        default:
            // Default case (should not happen)
            return 0;
    }
}

Board getPath(Node_State* win) {
    if (win == nullptr) {
        std::cerr << "Error: win is null.\n";
        return Board(0, 0);
    }

    //set a board to edit on it
    Board path(win->getBoard());
    Node_State* current = win;

    cout << "Tracing back the path...\n";
    while (current != nullptr) {
        Tile playerTile = current->getBoard().getPlayerTile();
        path.getTile(playerTile.getRow(), playerTile.getCol()).setValue(8);

        // Move up to parent
        Node_State* parent = current->getParent();
        if (parent == nullptr) break; // Exit if there's no parent

        Tile parentTile = parent->getBoard().getPlayerTile();

        //Color the path between playerTile and parentTile
        if (playerTile.getRow() == parentTile.getRow()) { // Same row
            int startCol = min(playerTile.getCol(), parentTile.getCol());
            int endCol = max(playerTile.getCol(), parentTile.getCol());
            for (int i = startCol; i <= endCol; i++) {
                path.getTile(playerTile.getRow(), i).setValue(8);
            }
        } else if (playerTile.getCol() == parentTile.getCol()) { // Same column
            int startRow = min(playerTile.getRow(), parentTile.getRow());
            int endRow = max(playerTile.getRow(), parentTile.getRow());
            for (int i = startRow; i <= endRow; i++) {
                path.getTile(i, playerTile.getCol()).setValue(8);
            }
        }

        current = parent; // Move to the next node
        /* printBoard(path);
        system("sleep 3s; clear"); */
    }

    cout << "Path tracing completed.\n";
    return path;
}


string stringBoard(Board& b) {
    string boardHash;
    int n = b.getN();
    int m = b.getM();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            boardHash += to_string(b.getTile(i, j).getValue());
            boardHash += ',';
        }
    }
    return boardHash;
}
