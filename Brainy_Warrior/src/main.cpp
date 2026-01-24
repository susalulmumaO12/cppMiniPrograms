#include <iostream>
#include <ncurses.h>

#include "brainy_warrior_tui.h"
#include "main.h"
#include "helper_functions.h"

// print-strings all in one place
const std::string BrainyWarrior = "__________               .__                __      __                     .__              \n\\______   \\____________  |__| ____ ___.__. /  \\    /  \\_____ ______________|__| ___________ \n |    |  _/\\_  __ \\__  \\ |  |/    <   |  | \\   \\/\\/   /\\__  \\_  __ \\_  __ \\  |/  _ \\_  __  \\\n |    |   \\ |  | \\// __ \\|  |   |  \\___  |  \\        /  / __ \\|  | \\/|  | \\/  (  <_> )  | \\/\n |______  / |__|  (____  /__|___|  / ____|   \\__/\\  /  (____  /__|   |__|  |__|\\____/|__|   \n        \\/             \\/        \\/\\/             \\/        \\/                              ";
const std::string algorithms = "\033[38;5;189mChoose algorithm: \033[0m\n\033[48;5;220m\033[38;5;18m1) BFS\033[0m\033[0m \033[48;5;17m\033[38;5;220m2) DFS\033[0m\033[0m \033[48;5;175m\033[38;5;53m3) UCS\033[0m\033[0m \033[48;5;28m\033[38;5;52m4) Hill Climbing\033[0m\033[0m \033[48;5;160m\033[38;5;231m5) A_star\033[0m\033[0m";

int main() {
	
    init_tui();

    while (true) {

        int choice = main_menu();
        if (choice == 0) {
            GameSession::getInstance().playerName = ask_name();
            levels_menu();
        } // LEVELS
        else if (choice == 1) computer_play(); // COMPUTER
        else if (choice == 2) stats_menu(); // STATS
        else { quit(); break; } // QUIT
    }

    return 0;
}

#include "algorithms.h"

void computer_play() {
    std::string levelKey = choose_level_menu();
    if (levelKey.empty()) return;

    int algo = choose_algorithm_menu();
    if (algo == -1) return;

    Board board = set_game_board(levelKey);
    clear();
    printBoard(board);
    refresh();

    switch(algo) {
        case 0: bfs(board); break;
        case 1: dfs(board); break;
        case 2: ucs(board); break;
        case 3: hill_climbing(board); break;
        case 4: a_star(board); break;
    }
}
