#include "helper_functions.h"
#define DEBUG false

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <list>
#include <vector>
#include <cmath>
#include <algorithm>
#include "node_state.h"
#include "board.h"
#include "tile.h"
#include "main.h"
#include "moves.h"
#include <ncurses.h>
#include "brainy_warrior_tui.h"

using json = nlohmann::json;

using namespace std;

Board set_game_board(std::string levelKey){
    ifstream inputFile("../src/levels.json");
    if (!inputFile.is_open()) {
        debug_log("Error opening levels.json file!");
        return Board(0, 0);
    }

    // create an empty structure (null)
    json j;
    inputFile >> j;

    if (j.find(levelKey) == j.end()) {
        debug_log("Level " + levelKey + " not found!");
        return Board(0, 0);
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


void printTile(int value, int r, int c) {
    int pair = 0;
    const char* sym = " ? ";
    switch (value) {
        case 0: pair = SEA_PAIR; sym = "   "; break;
        case 1: pair = LAND_PAIR; sym = " . "; break;
        case 3: pair = WIZARD_TILE_PAIR; sym = " O "; break;
        case 4: pair = WALL_PAIR; sym = " # "; break;
        case 5: pair = TARGET_PAIR; sym = " X "; break;
        case 6: pair = STAR_PAIR; sym = " * "; break;
        case 7: pair = WIZARD_PAIR; sym = " W "; break;
        case 8: pair = WON_PLAYER_PAIR; sym = " @ "; break;
        case 9: pair = PLAYER_PAIR; sym = " P "; break;
        case -1: pair = DEAD_PLAYER_PAIR; sym = " X "; break;
        default: sym = " ? "; break;
    }
    attron(COLOR_PAIR(pair));
    mvprintw(r, c, "%s", sym);
    attroff(COLOR_PAIR(pair));
}

void printBoard(Board board){
    int n = board.getN();
    int m = board.getM();
    int start_r = (LINES - n) / 2;
    int start_c = (COLS - m * 3) / 2;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printTile(board.getTile(i, j).getValue(), start_r + i, start_c + j * 3);
        }
    }
    refresh();
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
    
    if (!stats["levels"].contains(levelName)) {
        debug_log("No one played this level yet!");
    } else {
        for (auto player : stats["levels"][levelName].items()) {
            string playerName = player.key();
            int playerScore = player.value()["score"];
            int playerTries = player.value()["tries"];
            int playerWins = player.value()["wins"];
            score_pairs.push_back({playerScore, {playerTries, {playerWins, playerName}}});
        }
        sort(score_pairs.begin(), score_pairs.end());
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
            debug_log("Error parsing stats.json. Initializing a new file.");
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

    int currentScore = GameSession::getInstance().score;
    if (!stats["levels"][levelName][playerName].contains("score") || 
        (currentScore < stats["levels"][levelName][playerName]["score"].get<int>() ||
        stats["levels"][levelName][playerName]["score"].get<int>() == 0)) {
        debug_log("New best score for " + playerName + "! Score: " + to_string(currentScore));
        stats["levels"][levelName][playerName]["score"] = currentScore;
        
        // Visual feedback for new best score
        attron(A_BOLD | COLOR_PAIR(STAR_PAIR));
        mvprintw((LINES + 7) / 2 + 4, (COLS - 30) / 2, " NEW BEST SCORE: %d! ", currentScore);
        attroff(A_BOLD | COLOR_PAIR(STAR_PAIR));
    }

    //save the updated stats
    ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        outputFile << stats.dump(4);
        outputFile.close();
    } else {
        debug_log("Error: Unable to write to stats.json");
    }
}

dist getDistanceType() {
    int choice;
    // This is currently unused in the main flow but kept for compatibility
    return man; 
}

list<shared_ptr<Node_State>> get_next_cost_states(shared_ptr<Node_State> board){
    list<shared_ptr<Node_State>> states;
    
    if(canMoveUp(board->getBoard())){ //cost of moving up: 1
        Board up(board->getBoard());
        up = slideUp(up);
        if(up.getPlayerTile().getValue() != -1){

            auto up_state = make_shared<Node_State>(up, board, board->getG() + 1, 0);
            states.push_back(up_state);
            if(DEBUG){
                debug_log("Generated UP state");
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


            auto down_state = make_shared<Node_State>(down, board, board->getG() + 2, 0);
            states.push_back(down_state);
            if(DEBUG){
                debug_log("Generated DOWN state");
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


            auto left_state = make_shared<Node_State>(left, board, board->getG() + 5, 0);
            states.push_back(left_state);

            if(DEBUG){
                debug_log("Generated LEFT state");
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


            auto right_state = make_shared<Node_State>(right, board, board->getG() + 3, 0);
            states.push_back(right_state);
            if(DEBUG){
                debug_log("Generated RIGHT state");
            }

            if(right.win()){
                return states;
            }
        }
    }

    // Wizard moves are always allowed if wizard tiles exist
    if (!board->getBoard().getWizardTiles().empty()) {
        Board wiz(board->getBoard());
        wiz = moveWizard(wiz);
        if (stringBoard(wiz) != stringBoard(board->getBoard())) {
            auto wiz_state = make_shared<Node_State>(wiz, board, board->getG() + 1, 0);
            states.push_back(wiz_state);
            if(DEBUG){
                debug_log("Generated WIZARD state");
            }
            if(wiz.win()){
                return states;
            }
        }
    }

    return states;
}

int distance(Tile t1, Tile t2) {
    int minkowskiP = 3;
    vector<int> v1 = {t1.getRow(), t1.getCol()};
    vector<int> v2 = {t2.getRow(), t2.getCol()};

    switch (GameSession::getInstance().distanceType) {
        case man:
            return abs(t1.getRow() - t2.getRow()) + abs(t1.getCol() - t2.getCol());
        case euc:
            return static_cast<int>(sqrt((t1.getRow() - t2.getRow()) * (t1.getRow() - t2.getRow()) +
                                          (t1.getCol() - t2.getCol()) * (t1.getCol() - t2.getCol())));
        case mink: {
            double sum = 0.0;
            for (size_t i = 0; i < v1.size(); ++i) {
                sum += pow(abs(v1[i] - v2[i]), minkowskiP);
            }
            return static_cast<int>(pow(sum, 1.0 / minkowskiP));
        }
        case cheb: {
            return static_cast<int>(max(abs(t2.getRow() - t1.getRow()), abs(t2.getCol() - t1.getCol())));
        }
        default:
            return 0;
    }
}

Board getPath(shared_ptr<Node_State> win) {
    if (win == nullptr) {
        debug_log("Error: win is null.");
        return Board(0, 0);
    }

    Board path(win->getBoard());
    shared_ptr<Node_State> current = win;

    while (current != nullptr) {
        Tile playerTile = current->getBoard().getPlayerTile();
        if (playerTile.getRow() >= 0 && playerTile.getRow() < path.getN() &&
            playerTile.getCol() >= 0 && playerTile.getCol() < path.getM()) {
            path.getTile(playerTile.getRow(), playerTile.getCol()).setValue(8);
        }

        shared_ptr<Node_State> parent = current->getParent();
        if (parent == nullptr) break;

        Tile parentTile = parent->getBoard().getPlayerTile();

        if (playerTile.getRow() == parentTile.getRow()) { 
            int startCol = min(playerTile.getCol(), parentTile.getCol());
            int endCol = max(playerTile.getCol(), parentTile.getCol());
            for (int i = startCol; i <= endCol; i++) {
                path.getTile(playerTile.getRow(), i).setValue(8);
            }
        } else if (playerTile.getCol() == parentTile.getCol()) {
            int startRow = min(playerTile.getRow(), parentTile.getRow());
            int endRow = max(playerTile.getRow(), parentTile.getRow());
            for (int i = startRow; i <= endRow; i++) {
                path.getTile(i, playerTile.getCol()).setValue(8);
            }
        }
        current = parent;
    }
    return path;
}


string stringBoard(Board& b) {
    int n = b.getN();
    int m = b.getM();
    string boardHash;
    boardHash.reserve(n * m + 3); // board + wizard state

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int val = b.getTile(i, j).getValue();
            // Map common values to single chars for compression
            if (val >= 0 && val <= 9) boardHash += (char)('0' + val);
            else if (val == -1) boardHash += 'X';
            else boardHash += '?';
        }
    }
    // Include wizard state in hash to distinguish states where wizard moved
    boardHash += b.isWizardUsed() ? 'W' : 'w';
    boardHash += (char)('A' + b.getCurrentWizardIndex() + 1);
    return boardHash;
}

void debug_log(const std::string& message) {
    std::ofstream logFile("debug.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.close();
    }
}
