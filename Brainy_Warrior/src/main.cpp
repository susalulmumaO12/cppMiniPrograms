#include <iostream>
#include "algorithms.h"
#include "tile.h"
#include "board.h"
#include "helper_functions.h"
#include "moves.h"
#include "main.h"
#include <queue>
#include <ncurses.h>
#include <curses.h>
#include <menu.h>


// defining default value for each global variable
bool WIZMOVED = false;
circularq WIZARDTILES;
Tile currentWizard;
int SCORE = 0;
dist distanceType = man;
std::string name;

const char *main_options[] = {"Play Levels", "View Stats", "Quit Game", nullptr};
#define BANNER_PAIR 1
#define OPTION_PAIR 2
#define SELECTED_OPTION_PAIR 3

// print-strings all in one place
const std::string BrainyWarrior = "__________               .__                __      __                     .__              \n\\______   \\____________  |__| ____ ___.__. /  \\    /  \\_____ ______________|__| ___________ \n |    |  _/\\_  __ \\__  \\ |  |/    <   |  | \\   \\/\\/   /\\__  \\_  __ \\_  __ \\  |/  _ \\_  __  \\\n |    |   \\ |  | \\// __ \\|  |   |  \\___  |  \\        /  / __ \\|  | \\/|  | \\/  (  <_> )  | \\/\n |______  / |__|  (____  /__|___|  / ____|   \\__/\\  /  (____  /__|   |__|  |__|\\____/|__|   \n        \\/             \\/        \\/\\/             \\/        \\/                              ";
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

int custom_menu(const char* title, const char **options) {
    clear();

    ITEM **items = new ITEM*[100];
    int numOptions = 0;
    while (options[numOptions]) {
        items[numOptions] = new_item(options[numOptions], "");
        numOptions++;
    }
    items[numOptions] = nullptr;

    MENU *menu = new_menu((ITEM **)items);

    // Set appearance of menu items
    set_menu_fore(menu, COLOR_PAIR(SELECTED_OPTION_PAIR));
    set_menu_back(menu, COLOR_PAIR(OPTION_PAIR));
    set_menu_pad(menu, COLOR_PAIR(MENU_PAD_PAIR));
    
    int winHeight = 10, winWidth = 40;
    int startx = (COLS - winWidth) / 2;
    int starty = (LINES - winHeight) / 2;
    WINDOW *menuWin = newwin(winHeight, winWidth, starty, startx);
    keypad(menuWin, TRUE);

    set_menu_win(menu, menuWin);
    set_menu_sub(menu, derwin(menuWin, 6, 38, 3, 1));
    set_menu_mark(menu, " > ");

    /* current menu looks like this
    
                                            ┌──────────┤   MAIN  MENU   ├──────────┐
                                            │                                      │
                                            │                                      │
                                            │ > Play Levels                        │
                                            │   View Stats                         │
                                            │   Quit Game                          │
                                            │                                      │
                                            │                                      │
                                            │                                      │
                                            └──────────────────────────────────────┘

    */
    box(menuWin, 0, 0);
    mvwprintw(menuWin, 0, (40 - strlen(title)) / 2, "%s", title);
	mvwaddch(menuWin, 0, 11, ACS_RTEE);
	//mvwhline(menuWin, 0, 1, ACS_HLINE, 38);
	mvwaddch(menuWin, 0, 28, ACS_LTEE);

    post_menu(menu);
    wrefresh(menuWin);


    int choice = -1;
    int c;
    while ((c = wgetch(menuWin)) != '\n') {
        switch (c) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
        }
        wrefresh(menuWin);
    }

    int choice = item_index(current_item(menu));

    unpost_menu(menu);
    free_menu(menu);
    for (int j = 0; j < numOptions; ++j) {
        free_item(items[j]);
    }
    delete[] items;
    delwin(menuWin);

    return choice;
}

int main() {
	/* Initialize curses */	
	initscr();
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE); // support arrow keys for stdscr
    curs_set(0);
	
    init_pair(BANNER_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(OPTION_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SELECTED_OPTION_PAIR, COLOR_BLUE, COLOR_YELLOW);
    init_pair(MENU_PAD_PAIR, COLOR_BLUE, COLOR_YELLOW);

    attron(COLOR_PAIR(BANNER_PAIR));
    mvaddstr(0, 0, BrainyWarrior.c_str());
    attroff(COLOR_PAIR(BANNER_PAIR));

    while (true) {
        int choice = custom_menu("   MAIN  MENU   ", main_options);
        if (choice == 0) levels();
        else if (choice == 1) stats();
        else break;
    }

    endwin();

    switch (choice) {
        case 1: levels(); break;
        case 2: stats(); break;
        case 3: std::exit(0); break;
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