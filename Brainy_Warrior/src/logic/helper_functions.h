#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#define DEBUG false

#include<iostream>
#include<list>
#include<vector>
#include<cmath>
#include"../structure/node_state.h"
#include"../structure/board.h"
#include"../structure/tile.h"
#include"../include/main.h"
#include"moves.h"

using json = nlohmann::json;

using namespace std;

void updateScore(const string& playerName, int level) {
    json scores;

    // Load existing scores
    ifstream inputFile("../scores.json");
    if (inputFile.is_open()) {
        inputFile >> scores;
        inputFile.close();
    }
    string levelName = "level " + to_string(level);
    string method = SLIDE? "slide" : "move";

    // Check and update the score
    if (!scores["levels"][levelName].contains(playerName)) {
        scores["levels"][levelName][playerName][method] = SCORE;
    } 
void updateScore(const string& playerName, int level) {
    json scores;
    string filePath = "../scores.json";

    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        //file doesn't exist, create it with an empty JSON object
        ofstream outputFile(filePath);
        if (outputFile.is_open()) {
            //initialize with empty JSON
            outputFile << "{}";
            outputFile.close();
            }
        } else {
        try {
            inputFile >> scores;
        } catch (json::parse_error& e) {
            //if the file is empty or has invalid JSON, initialize it
            cerr << "Error parsing scores.json, reinitializing file." << endl;
            scores = json::object();
        }
        inputFile.close();
    }

    // Update the player's score
    string levelName = "level " + to_string(level);

    // Ensure the structure exists and update the score
    if (!scores.contains("levels")) {
        scores["levels"] = json::object();
        }
    if (!scores["levels"].contains(levelName)) {
        scores["levels"][levelName] = json::object();
    }
    if (!scores["levels"][levelName].contains(playerName) || 
        scores["levels"][levelName][playerName]["score"].get<int>() < SCORE) {
        scores["levels"][levelName][playerName]["score"] = SCORE;
        cout << "New high score for " << playerName << " on " << levelName << ": " << SCORE << endl;
    } else {
        cout << playerName << "'s score remains: " 
             << scores["levels"][levelName][playerName]["score"] << endl;
    }

    // Save the updated JSON back to the file
    ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        outputFile << scores.dump(4);
        outputFile.close();
    } else {
        cerr << "Error: Unable to write to scores.json" << endl;
    }
}

list<Node_State> get_next_cost_states(Node_State* board){
    list<Node_State> states;
    
    if(canMoveUp(board->getBoard())){ //cost of moving up: 1
        Board up(board->getBoard());
        up = !SLIDE? moveUp(up)  : slideUp(up);
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
        down = !SLIDE? moveDown(down) : slideDown(down);
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
        left = !SLIDE? moveLeft(left) : slideLeft(left);
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
        right = !SLIDE? moveRight(right) : slideRight(right);
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

#endif