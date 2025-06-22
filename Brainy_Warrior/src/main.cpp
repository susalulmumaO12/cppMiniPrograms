#include <iostream>

#include "brainy_warrior_tui.h"
#include "main.h"

// defining default value for each global variable
bool WIZMOVED = false;
circularq WIZARDTILES;
Tile currentWizard;
int SCORE = 0;
dist distanceType = man;
std::string name;


// print-strings all in one place
const std::string BrainyWarrior = "__________               .__                __      __                     .__              \n\\______   \\____________  |__| ____ ___.__. /  \\    /  \\_____ ______________|__| ___________ \n |    |  _/\\_  __ \\__  \\ |  |/    <   |  | \\   \\/\\/   /\\__  \\_  __ \\_  __ \\  |/  _ \\_  __  \\\n |    |   \\ |  | \\// __ \\|  |   |  \\___  |  \\        /  / __ \\|  | \\/|  | \\/  (  <_> )  | \\/\n |______  / |__|  (____  /__|___|  / ____|   \\__/\\  /  (____  /__|   |__|  |__|\\____/|__|   \n        \\/             \\/        \\/\\/             \\/        \\/                              ";
const std::string algorithms = "\033[38;5;189mChoose algorithm: \033[0m\n\033[48;5;220m\033[38;5;18m1) BFS\033[0m\033[0m \033[48;5;17m\033[38;5;220m2) DFS\033[0m\033[0m \033[48;5;175m\033[38;5;53m3) UCS\033[0m\033[0m \033[48;5;28m\033[38;5;52m4) Hill Climbing\033[0m\033[0m \033[48;5;160m\033[38;5;231m5) A_star\033[0m\033[0m";

int main() {
	
    init_tui();

    while (true) {

        int choice = main_menu();
        if (choice == 0) {
            name = ask_name();
            levels_menu();
        } // LEVELS
        //else if (choice == 1) computer_play(); // COMPUTER
        else if (choice == 2) stats_menu(); // STATS
        else { quit(); break; } // QUIT
    }

    return 0;
}

void computer_play() {
    // TODO deal with this
    /* distanceType = getDistanceType();
        int algorithm;
        std::cout<<algorithms<<std::endl;
        std::cin>>algorithm;
        switch(algorithm) {
            case 1: bfs(board); break;
            case 2: dfs(board); break;
            case 3: ucs(board); break;
            case 4: hill_climbing(board); break;
            case 5: a_star(board); break;
        } */
}