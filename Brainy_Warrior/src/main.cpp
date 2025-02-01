#include <iostream>
#include "algorithms.h"
#include "tile.h"
#include "board.h"
#include "helper_functions.h"
#include "moves.h"
#include "main.h"
#include <queue>

// defining default value for each global variable
bool WIZMOVED = false;
circularq WIZARDTILES;
Tile currentWizard;
int SCORE = 0;
dist distanceType = man;
std::string name;

// print-strings all in one place
const std::string BrainyWarrior = "\033[38;5;33m\n\n__________               .__                __      __                     .__              \n\\______   \\____________  |__| ____ ___.__. /  \\    /  \\_____ ______________|__| ___________ \n |    |  _/\\_  __ \\__  \\ |  |/    <   |  | \\   \\/\\/   /\\__  \\_  __ \\_  __ \\  |/  _ \\_  __  \\\n |    |   \\ |  | \\// __ \\|  |   |  \\___  |  \\        /  / __ \\|  | \\/|  | \\/  (  <_> )  | \\/\n |______  / |__|  (____  /__|___|  / ____|   \\__/\\  /  (____  /__|   |__|  |__|\\____/|__|   \n        \\/             \\/        \\/\\/             \\/        \\/                              \n\n\033[1m";
const std::string nameQuestion = "What's your name?";
const std::string mainMenu = "1) Levels\n2) Stats\n3) Quit";
const std::string levelsQuestion = "\033[38;5;33mEnter a level number between 1 and 17: \033[1m";
const std::string invalidLevel = "\033[38;5;9mInvalid level number!\033[0m";
const std::string playingOptions = "\033[38;5;226mPlaying options: \033[0m\n\033[48;5;208m\033[38;5;231m1) User\033[0m\033[0m\n\033[48;5;33m\033[38;5;231m2) Computer\033[0m\033[0m\n3) Back to main menu";
const std::string instructions = "Type in a letter using either of the following ways: WASD, IJKL, Z for wizard clockwise movement";
const std::string gameOver = "\033[31mGAME OVER! You drowned...\033[0m";
const std::string gameWin = "\033[38;5;226mYOU WIN!\033[0m";
const std::string algorithms = "\033[38;5;189mChoose algorithm: \033[0m\n\033[48;5;220m\033[38;5;18m1) BFS\033[0m\033[0m \033[48;5;17m\033[38;5;220m2) DFS\033[0m\033[0m \033[48;5;175m\033[38;5;53m3) UCS\033[0m\033[0m \033[48;5;28m\033[38;5;52m4) Hill Climbing\033[0m\033[0m \033[48;5;160m\033[38;5;231m5) A_star\033[0m\033[0m";

void levels();
void stats();

int main() {
    
    std::cout<<BrainyWarrior;
    std::cout<<nameQuestion<<std::endl;
    std::cin>>name;

    std::cout<<mainMenu<<std::endl;
    int mainMenuChoice;
    std::cin>>mainMenuChoice;

    switch(mainMenuChoice) {
        case 1:
            levels(); break;
        case 2:
            stats(); break;
        case 3:
            exit(0); break;

    }
        
    return 0;
}

void levels() {
    // get user input for level number
    int levelChoice;
    std::cout<<levelsQuestion;
    std::cin>>levelChoice;

    // is level number valid?
    if (levelChoice < 1 || levelChoice > 17) {
        std::cerr<<invalidLevel<<std::endl;
    }

    Board board = set_game_board(levelChoice);
    
    int playingOption;
    std::cout<<playingOptions<<std::endl;
    std::cin>>playingOption;

    WIZARDTILES = getWizardTiles(board);

    std::cout << "\033[38;5;226mBoard for level \033[0m " << levelChoice << ":\n";
    printBoard(board);

    // USER PLAYING
    if(playingOption == 1){

        std::cout<<instructions<<std::endl;
        Node_State state(board, nullptr, 0, 0);
        std::queue<Node_State> q;
        q.push(state);

        while(!q.empty()){
            Tile& player = board.getPlayerTile();
            Node_State current(q.front());
            q.pop();

            // WIN CASE
            if(board.win()){
                std::cout<<gameWin<<std::endl;
                updateStats(name, levelChoice, true);
                std::cout<<"print path? (y/n)\n";
                char yn1; std::cin>>yn1;
                if(tolower(yn1) == 'y') printBoard(getPath(&current));

                std::cout<< "Print stats? (y/n)\n";
                char yn2; std::cin>>yn2;
                tolower(yn2) == 'y' ? printStats(levelChoice) : exit(0);
                exit(0);
            }

            // LOSE CASE
            if(player.getValue() == -1){
                std::cout<<gameOver<<std::endl;
                updateStats(name, levelChoice, false);
            }

            char m; std::cin>>m;
            board = move(board, m);
            Node_State movedBoard(board, &current, 0, 0);
            q.push(movedBoard);
            printBoard(board);

        }
    // COMPUTER PLAYING
    } else if (playingOption == 2){

        distanceType = getDistanceType();
        int algorithm;
        std::cout<<algorithms<<std::endl;
        std::cin>>algorithm;
        switch(algorithm) {
            case 1: bfs(board); break;
            case 2: dfs(board); break;
            case 3: ucs(board); break;
            case 4: hill_climbing(board); break;
            case 5: a_star(board); break;
        }
    } else {
        // implement back to main menu
    }
}

void stats() {
    printStats(0);
}