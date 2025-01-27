#include <iostream>
#include "algorithms.h"
#include "tile.h"
#include "board.h"
#include "helper_functions.h"
#include "moves.h"
#include "main.h"

// defining default value for each global variable
bool SLIDE = false;
bool WIZMOVED = false;
circularq WIZARDTILES;
Tile currentWizard;
int SCORE = 0;
dist distanceType = man;

// print-strings all in one place
const std::string BrainyWarrior = "\033[38;5;33m\n\n__________               .__                __      __                     .__              \n\\______   \\____________  |__| ____ ___.__. /  \\    /  \\_____ ______________|__| ___________ \n |    |  _/\\_  __ \\__  \\ |  |/    <   |  | \\   \\/\\/   /\\__  \\_  __ \\_  __ \\  |/  _ \\_  __  \\\n |    |   \\ |  | \\// __ \\|  |   |  \\___  |  \\        /  / __ \\|  | \\/|  | \\/  (  <_> )  | \\/\n |______  / |__|  (____  /__|___|  / ____|   \\__/\\  /  (____  /__|   |__|  |__|\\____/|__|   \n        \\/             \\/        \\/\\/             \\/        \\/                              \n\n\033[1m";
const std::string nameQuestion = "What's your name?";
const std::string levelsQuestion = "\033[38;5;33mEnter a level number between 1 and 17: \033[1m";
const std::string invalidLevel = "\033[38;5;9mInvalid level number!\033[0m";
const std::string playingOptions = "\033[38;5;226mPlaying options: \033[0m\n\033[48;5;208m\033[38;5;231m1) User\033[0m\033[0m\n\033[48;5;33m\033[38;5;231m2) Computer\033[0m\033[0m";
const std::string instructions = "Type in a letter using either of the following ways: WASD, IJKL, Z for wizard clockwise movement";
const std::string gameOver = "\033[31mGAME OVER! You drowned...\033[0m";
const std::string gameWin = "\033[38;5;226mYOU WIN!\033[0m";
const std::string algorithms = "\033[38;5;189mChoose algorithm: \033[0m\n\033[48;5;220m\033[38;5;18m1) BFS\033[0m\033[0m \033[48;5;17m\033[38;5;220m2) DFS\033[0m\033[0m \033[48;5;175m\033[38;5;53m3) UCS\033[0m\033[0m \033[48;5;28m\033[38;5;52m4) Hill Climbing\033[0m\033[0m \033[48;5;160m\033[38;5;231m5) A_star\033[0m\033[0m";
int main() {
    
    std::cout<<BrainyWarrior;
    std::cout<<nameQuestion<<std::endl;
    std::string name; std::cin>>name;

    // get user input for level number
    int levelChoice;
    std::cout<<levelsQuestion;
    std::cin>>levelChoice;

    // is level number valid?
    if (levelChoice < 1 || levelChoice > 17) {
        std::cerr<<invalidLevel<<std::endl;
        return 1;
    }

    Board board = set_game_board(levelChoice);
    
    int playingOption;
    std::cout<<playingOptions<<std::endl;
    std::cin>>playingOption;

    WIZARDTILES = getWizardTiles(board);

    // TODO: remove this
    int difficulty;
    std::cout<<"\033[38;5;42mChoose difficulty level:\033[0m \n\033[48;5;30m\033[38;5;231m1) Easy (tile move style)\033[0m\033[0m\n\033[48;5;161m\033[38;5;231m2) Hard (slide style)\033[0m\033[0m\n";
    std::cin>>difficulty;
    difficulty == 1? SLIDE = false : SLIDE = true;

    std::cout << "\033[38;5;226mBoard for level \033[0m " << levelChoice << ":\n";
    printBoard(board);

    // USER PLAYING
    if(playingOption == 1){

        std::cout<<instructions<<std::endl;

        while(!board.win()){
            Tile& player = board.getPlayerTile();

            // LOSE CASE
            if(player.getValue() == -1){
                std::cout<<gameOver<<std::endl;
                updateStats(name, levelChoice, false);
                return 0;
            }

            char m; std::cin>>m;
            board = move(board, m);
            printBoard(board);

            // WIN CASE
            if(board.win()){
                std::cout<<gameWin<<std::endl;
                updateStats(name, levelChoice, true);

                std::cout<< "Print scores? (y/n)\n";
                char yn; std::cin>>yn;
                tolower(yn) == 'y' ? printScores(levelChoice) : exit(0);
                return 0;
            }
        }
    // COMPUTER PLAYING
    } else if (playingOption == 2){

        distanceType = getDistanceType();
        int algorithm;
        std::cout<<algorithms<<std::endl;
        std::cin>>algorithm;
        if(algorithm == 1) bfs(board);
        else if(algorithm == 2) dfs(board);
        else if(algorithm == 3) ucs(board);
        else if(algorithm == 4) hill_climbing(board);
        else if(algorithm == 5) a_star(board);
    }
    
    return 0;
}
