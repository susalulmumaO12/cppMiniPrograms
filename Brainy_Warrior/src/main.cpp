#include <iostream>
#include "../include/main.h"
#include "./structure/tile.h"
#include "./structure/board.h"
#include "./logic/helper_functions.h"
#include "./logic/moves.h"
#include "./game_solver/breadth_first_search.h"
#include "./game_solver/depth_first_search.h"
#include "./game_solver/uniform_cost_search.h"
#include "./game_solver/A_star_search.h"
#include "./game_solver/hill_climbing.h"

using namespace std;

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

void printTile(int value) {
    switch (value) {
        case 0: cout << "\033[48;5;45m\033[38;5;75m 0 \033[0m\033[0m"; break;  // 0:sea
        case 1: cout << "\033[48;5;42m\033[38;5;36m 1 \033[0m\033[0m"; break;  // 1:land
        case 3: cout << "\033[48;5;22m\033[38;5;165m O \033[0m\033[0m"; break;  // 3:wizard tile
        case 4: cout << "\033[48;5;226m\033[38;5;220m 4 \033[0m\033[0m"; break;  // 4:wall
        case 5: cout << "\033[48;5;96m\033[1;38;5;89m 5 \033[0m\033[0m"; break;  // 5:target
        case 6: cout << "\033[38;5;226m * \033[0m"; break;  // 6:star
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

int main() {

    cout<<"\033[38;5;33m\n\n__________               .__                __      __                     .__              \n\\______   \\____________  |__| ____ ___.__. /  \\    /  \\_____ ______________|__| ___________ \n |    |  _/\\_  __ \\__  \\ |  |/    <   |  | \\   \\/\\/   /\\__  \\_  __ \\_  __ \\  |/  _ \\_  __  \\\n |    |   \\ |  | \\// __ \\|  |   |  \\___  |  \\        /  / __ \\|  | \\/|  | \\/  (  <_> )  | \\/\n |______  / |__|  (____  /__|___|  / ____|   \\__/\\  /  (____  /__|   |__|  |__|\\____/|__|   \n        \\/             \\/        \\/\\/             \\/        \\/                              \n\n\033[1m";
    
    // get user input for level number
    int levelChoice;
    cout<<"\033[38;5;33mEnter a level number between 1 and 16: \033[1m";
    cin>>levelChoice;

    // is level number valid?
    if (levelChoice < 1 || levelChoice > 17) {
        cerr<<"\033[38;5;9mInvalid level number!\033[0m"<<endl;
        return 1;
    }

    Board board = set_game_board(levelChoice);
    
    int playingOption;
    cout<<"\033[38;5;226mPlaying options: \033[0m\n\033[48;5;208m\033[38;5;231m1) User\033[0m\033[0m\n\033[48;5;33m\033[38;5;231m2) Computer\033[0m\033[0m\n";
    cin>>playingOption;

    WIZARDTILES = getWizardTiles(board);

    int difficulty;
    cout<<"\033[38;5;42mChoose difficulty level:\033[0m \n\033[48;5;30m\033[38;5;231m1) Easy (tile move style)\033[0m\033[0m\n\033[48;5;161m\033[38;5;231m2) Hard (slide style)\033[0m\033[0m\n";
    cin>>difficulty;
    difficulty == 1? SLIDE = false : SLIDE = true;

    cout << "\033[38;5;226mBoard for level \033[0m " << levelChoice << ":\n";
    printBoard(board);

    // USER PLAYING
    if(playingOption == 1){
        

        cout<<"Type in a letter using either of the following ways: WASD, IJKL\n";

        while(!board.win()){
            Tile& player = board.getPlayerTile();

            if(player.getValue() == -1){
                cout<<"\033[31mGAME OVER! You drowned...\033[0m\n";
                return 0;
            }

            char m; cin>>m;
            board = move(board, m);
            printBoard(board);

            if(board.win()){
                cout<<"\033[38;5;226mYOU WIN!\033[0m\n";
                return 0;
            }
        }

    } else if (playingOption == 2){

        distanceType = getDistanceType();

        int algorithm;
        cout<<"\033[38;5;189mChoose algorithm: \033[0m\n\033[48;5;220m\033[38;5;18m1) BFS\033[0m\033[0m \033[48;5;17m\033[38;5;220m2) DFS\033[0m\033[0m \033[48;5;175m\033[38;5;53m3) UCS\033[0m\033[0m \033[48;5;28m\033[38;5;52m4) Hill Climbing\033[0m\033[0m \033[48;5;160m\033[38;5;231m5) A_star\033[0m\033[0m\n";
        cin>>algorithm;
        if(algorithm == 1) bfs(board);
        else if(algorithm == 2) dfs(board);
        else if(algorithm == 3) ucs(board);
        else if(algorithm == 4) hill_climbing(board);
        else if(algorithm == 5) a_star(board);
    }
    
    return 0;
}
