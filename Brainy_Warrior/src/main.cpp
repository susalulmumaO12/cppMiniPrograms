#include <iostream>
#include <fstream>
#include <queue>
#include <locale.h>
#include <ncurses.h>
#include <curses.h>
#include <menu.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "algorithms.h"
#include "tile.h"
#include "board.h"
#include "helper_functions.h"
#include "moves.h"
#include "main.h"


using json = nlohmann::json;

// defining default value for each global variable
bool WIZMOVED = false;
circularq WIZARDTILES;
Tile currentWizard;
int SCORE = 0;
dist distanceType = man;
std::string name;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define BANNER_PAIR 1
#define OPTION_PAIR 2
#define SELECTED_OPTION_PAIR 3
#define GREY_OPTION_PAIR 4

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

void level(const std::string& level);
void stats();

int main_menu() {
    clear();
    const char *choices[] = {"Play Levels", "Computer Play","View Stats", "Quit Game"};
    
    ITEM **items;
    int c;				
	MENU *menu;
    WINDOW *menuWin;
    int n_choices;
    int winHeight = 10, winWidth = 40;
    int startx = (COLS - winWidth) / 2;
    int starty = (LINES - winHeight) / 2;
    const char* title = "   MAIN  MENU   ";

    n_choices = ARRAY_SIZE(choices);
    items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i)
        items[i] = new_item(choices[i], "");
	items[n_choices] = (ITEM *)NULL;
	menu = new_menu((ITEM **)items);
    menuWin = newwin(winHeight, winWidth, starty, startx);
    set_menu_fore(menu, COLOR_PAIR(SELECTED_OPTION_PAIR));
    set_menu_back(menu, COLOR_PAIR(OPTION_PAIR));
    set_menu_mark(menu, " > ");
    set_menu_win(menu, menuWin);
    set_menu_sub(menu, derwin(menuWin, 6, 38, 3, 1));

    keypad(menuWin, TRUE);
    box(menuWin, 0, 0);
	mvwprintw(menuWin, 0, (40 - strlen(title)) / 2, "%s", title);
	mvwaddch(menuWin, 0, 11, ACS_RTEE);
	//mvwhline(menuWin, 0, 1, ACS_HLINE, 38);
	mvwaddch(menuWin, 0, 28, ACS_LTEE);

    post_menu(menu);
    wrefresh(menuWin);

    while ((c = wgetch(menuWin)) != '\n') {
        switch (c) {
            case KEY_DOWN:  menu_driver(menu, REQ_DOWN_ITEM); break;
            case KEY_UP:    menu_driver(menu, REQ_UP_ITEM); break;
        }
        wrefresh(menuWin);
    }

    int choice = item_index(current_item(menu));

    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < n_choices; ++i)
        free_item(items[i]);
    delete[] items;
    delwin(menuWin);

    return choice;
}

void ask_name() {
    echo();
    char buffer[100] = {0};
    mvprintw((LINES - 18) / 2, (COLS - 18) / 2, "What's your name? ");
    refresh();
    
    getstr(buffer);
    name = buffer;
    name.erase(0, name.find_first_not_of(" \t\n\r\f\v"));
    name.erase(name.find_last_not_of(" \t\n\r\f\v") + 1);
    
    noecho();
    clear();
}

void levels_menu() {
    clear();
    ask_name();
    json levelsJson, statsJson;

    std::ifstream levelsFile("../src/levels.json");

    if (levelsFile.is_open()) {
        levelsFile >> levelsJson;
        levelsFile.close();
    }

    std::ifstream statsFile("../stats.json");
    if (statsFile.is_open()) {
        statsFile >> statsJson;
        statsFile.close();
    }

    std::vector<std::string> levelKeys;
    std::vector<std::string> labels;

    for (const auto& [levelName, info] : levelsJson.items()) {
        levelKeys.push_back(levelName);
        std::string label = levelName + " ";

        std::string difficulty = info.value("difficulty", "easy");
        bool hasWizard = info.contains("wizard") && info["wizard"].get<bool>();
        bool played = statsJson["levels"].contains(levelName) && statsJson["levels"][levelName].contains(name);

        if (difficulty == "easy") label += " ▂   "; 
        else if (difficulty == "medium") label += " ▂▃  ";
        else label += " ▂▃▄ ";
        
        label += hasWizard ? " ★ " : " ☆ ";
        label += played ? " ⚑ " : " ⚐ ";

        labels.push_back(label);
    }

    int cols = 2;
    int rows = (labels.size() + cols - 1) / cols;

    std::vector<const char*> choices;
    for (std::string const& str : labels) {
      choices.push_back(str.data());
    }

    ITEM **items, **backItems;
    int c;				
	MENU *menu, *back;
    WINDOW *menuWin;
    int n_choices;
    int winHeight = rows + 6;
    bool focusOnBack = false;

    int winWidth = 28 * cols + 2;
    int startx = (COLS - winWidth) / 2;
    int starty = (LINES - winHeight) / 2;
    const char* title = "  Choose a level  ";

    n_choices = choices.size();
    items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i)
        items[i] = new_item(choices[i], "");
	items[n_choices] = (ITEM *)NULL;

    backItems = (ITEM **)calloc(2, sizeof(ITEM *));
    const char* back_text = "< Back >";
    backItems[0] = new_item(back_text, "");
    backItems[1] = (ITEM *)NULL;

	menu = new_menu((ITEM **)items);
    menuWin = newwin(winHeight, winWidth, starty, startx);
    set_menu_format(menu, rows, cols);
    set_menu_fore(menu, COLOR_PAIR(SELECTED_OPTION_PAIR));
    set_menu_back(menu, COLOR_PAIR(OPTION_PAIR));
    set_menu_mark(menu, " > ");
    set_menu_win(menu, menuWin);
    set_menu_sub(menu, derwin(menuWin, rows, winWidth - 4, 2, 2));

    back = new_menu((ITEM **)backItems);
    set_menu_format(back, 1, 1);
    set_menu_fore(back, COLOR_PAIR(OPTION_PAIR));
    set_menu_back(back, COLOR_PAIR(OPTION_PAIR));
    set_menu_mark(back, "");
    set_menu_win(back, menuWin);
    set_menu_sub(back, derwin(menuWin, 1, strlen(back_text), winHeight - 2, 20));

    keypad(menuWin, TRUE);
    box(menuWin, 0, 0);
	mvwaddch(menuWin, 0, (winWidth - strlen(title)) / 3, ACS_RTEE);
	mvwaddch(menuWin, 0, winWidth - ((winWidth - strlen(title)) / 3), ACS_LTEE);
    mvwprintw(menuWin, 0, (winWidth - strlen(title)) / 2, "%s", title);

    post_menu(menu);
    post_menu(back);
    wrefresh(menuWin);


    while ((c = wgetch(menuWin)) != '\n') {

        if (c == 27) {
            focusOnBack = !focusOnBack;
            if (focusOnBack) {
                set_menu_fore(menu, COLOR_PAIR(GREY_OPTION_PAIR));
                set_menu_fore(back, COLOR_PAIR(SELECTED_OPTION_PAIR));
            } else {
                set_menu_fore(menu, COLOR_PAIR(SELECTED_OPTION_PAIR));
                set_menu_fore(back, COLOR_PAIR(OPTION_PAIR));
            }

            unpost_menu(menu);
            post_menu(menu);
            unpost_menu(back);
            post_menu(back);
        }
        if (focusOnBack) {
            menu_driver(back, REQ_DOWN_ITEM);
        } else {
            switch (c) {
                case KEY_DOWN:  menu_driver(menu, REQ_DOWN_ITEM); break;
                case KEY_UP:    menu_driver(menu, REQ_UP_ITEM); break;
                case KEY_LEFT:  menu_driver(menu, REQ_LEFT_ITEM); break;
                case KEY_RIGHT: menu_driver(menu, REQ_RIGHT_ITEM); break;
                case KEY_NPAGE: menu_driver(menu, REQ_SCR_DPAGE); break;
			    case KEY_PPAGE: menu_driver(menu, REQ_SCR_UPAGE); break;
        }
    }
        wrefresh(menuWin);
}

    int choice;

    unpost_menu(back);
    free_menu(back);
    free_item(backItems[0]);
    free(backItems);
    unpost_menu(menu);
    free_menu(menu);
    for (size_t i = 0; i < choices.size(); ++i)
        free_item(items[i]);
    free(items);
    werase(menuWin);
    wrefresh(menuWin);
    delwin(menuWin);
    clear();
    refresh();

    if (focusOnBack) {
        return;
    } else {
        choice = item_index(current_item(menu));
    }
    if (choice >= 0 && choice < (int)levelKeys.size()) {
        std::string levelKey = levelKeys[choice];
        clear();
        level(levelKey);
    }
}

void stats_menu() {
    
    json statsJson;
    std::ifstream statsFile("../stats.json");
    if (statsFile.is_open()) {
        statsFile >> statsJson;
        statsFile.close();
    }

    std::vector<std::string> labels;
    std::vector<std::string> levelKeys;

    labels.push_back("All stats");
    for (const auto& level : statsJson["levels"].items()) {
        std::string label = level.key() + " stats";
        levelKeys.push_back(levelName);
        labels.push_back(label);
    }

    int cols = 2;
    int rows = (labels.size() + cols - 1) / cols;

    std::vector<const char*> choices;
    for (std::string const& str : labels) {
      choices.push_back(str.data());
    }

    ITEM **items, **backItems;
    int c;				
	MENU *menu, *back;
    WINDOW *menuWin;
    int n_choices;
    int winHeight = rows + 6;
    bool focusOnBack = false;

    int winWidth = 20 * cols + 2;
    int startx = (COLS - winWidth) / 2;
    int starty = (LINES - winHeight) / 2;
    const char* title = "  STATS  ";

    n_choices = choices.size();
    items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i)
        items[i] = new_item(choices[i], "");
	items[n_choices] = (ITEM *)NULL;

    backItems = (ITEM **)calloc(2, sizeof(ITEM *));
    const char* back_text = "< Back >";
    backItems[0] = new_item(back_text, "");
    backItems[1] = (ITEM *)NULL;

	menu = new_menu((ITEM **)items);
    menuWin = newwin(winHeight, winWidth, starty, startx);
    set_menu_format(menu, rows, cols);
    set_menu_fore(menu, COLOR_PAIR(SELECTED_OPTION_PAIR));
    set_menu_back(menu, COLOR_PAIR(OPTION_PAIR));
    set_menu_mark(menu, " > ");
    set_menu_win(menu, menuWin);
    set_menu_sub(menu, derwin(menuWin, rows, winWidth - 4, 2, 2));

    back = new_menu((ITEM **)backItems);
    set_menu_format(back, 1, 1);
    set_menu_fore(back, COLOR_PAIR(OPTION_PAIR));
    set_menu_back(back, COLOR_PAIR(OPTION_PAIR));
    set_menu_mark(back, "");
    set_menu_win(back, menuWin);
    set_menu_sub(back, derwin(menuWin, 1, strlen(back_text), winHeight - 2, 20));

    keypad(menuWin, TRUE);
    box(menuWin, 0, 0);
	mvwaddch(menuWin, 0, (winWidth - strlen(title)) / 3, ACS_RTEE);
	mvwaddch(menuWin, 0, winWidth - ((winWidth - strlen(title)) / 3), ACS_LTEE);
    mvwprintw(menuWin, 0, (winWidth - strlen(title)) / 2, "%s", title);

    post_menu(menu);
    post_menu(back);
    wrefresh(menuWin);


    while ((c = wgetch(menuWin)) != '\n') {

        if (c == 27) {
            focusOnBack = !focusOnBack;
            if (focusOnBack) {
                set_menu_fore(menu, COLOR_PAIR(GREY_OPTION_PAIR));
                set_menu_fore(back, COLOR_PAIR(SELECTED_OPTION_PAIR));
            } else {
                set_menu_fore(menu, COLOR_PAIR(SELECTED_OPTION_PAIR));
                set_menu_fore(back, COLOR_PAIR(OPTION_PAIR));
            }

            unpost_menu(menu);
            post_menu(menu);
            unpost_menu(back);
            post_menu(back);
        }
        if (focusOnBack) {
            menu_driver(back, REQ_DOWN_ITEM);
        } else {
            switch (c) {
                case KEY_DOWN:  menu_driver(menu, REQ_DOWN_ITEM); break;
                case KEY_UP:    menu_driver(menu, REQ_UP_ITEM); break;
                case KEY_LEFT:  menu_driver(menu, REQ_LEFT_ITEM); break;
                case KEY_RIGHT: menu_driver(menu, REQ_RIGHT_ITEM); break;
                case KEY_NPAGE: menu_driver(menu, REQ_SCR_DPAGE); break;
			    case KEY_PPAGE: menu_driver(menu, REQ_SCR_UPAGE); break;
        }
    }
        wrefresh(menuWin);
}

int choice;

    unpost_menu(back);
    free_menu(back);
    free_item(backItems[0]);
    free(backItems);
    unpost_menu(menu);
    free_menu(menu);
    for (size_t i = 0; i < choices.size(); ++i)
        free_item(items[i]);
    free(items);
    werase(menuWin);
    wrefresh(menuWin);
    delwin(menuWin);
    clear();
    refresh();

    if (focusOnBack) {
        return;
    } else {
        choice = item_index(current_item(menu));
    }
    if (choice >= 0 && choice < (int)levelKeys.size()) {
        std::string levelKey = levelKeys[choice];
        clear();
        stats(levelKey);
    }

}

int main() {
	/* Initialize curses */	
    setlocale(LC_ALL, ""); // UTF-8 support
	initscr();
    set_escdelay(25);
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE); // support arrow keys for stdscr
    curs_set(0);
	
    init_pair(BANNER_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(OPTION_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SELECTED_OPTION_PAIR, COLOR_WHITE, COLOR_RED);
    init_pair(GREY_OPTION_PAIR, COLOR_BLACK, COLOR_WHITE); //TODO: look into color coding in various terminal emulators

    attron(COLOR_PAIR(BANNER_PAIR));
    mvaddstr(0, 0, BrainyWarrior.c_str());
    attroff(COLOR_PAIR(BANNER_PAIR));

    while (true) {

        int choice = main_menu();
        if (choice == 0) levels_menu(); // LEVELS
        //else if (choice == 1) computer_play(); // COMPUTER
        else if (choice == 2) stats_menu(); // STATS
        else break; // QUIT
    }

    endwin();
    return 0;
}

void level(const std::string& level) {
    Board board = set_game_board(level);
    
    WIZARDTILES = getWizardTiles(board);
    printBoard(board);

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
            updateStats(name, level, true);
            std::cout<<"print path? (y/n)\n";
            char yn1; std::cin>>yn1;
            if(tolower(yn1) == 'y') printBoard(getPath(&current));
            std::cout<< "Print stats? (y/n)\n";
            char yn2; std::cin>>yn2;
            tolower(yn2) == 'y' ? printStats(level) : exit(0);
            exit(0);
        }
        // LOSE CASE
        if(player.getValue() == -1){
            std::cout<<gameOver<<std::endl;
            updateStats(name, level, false);
        }
        char m; std::cin>>m;
        board = move(board, m);
        Node_State movedBoard(board, &current, 0, 0);
        q.push(movedBoard);
        printBoard(board);
    }
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
void stats() {
    //printStats("0"); // TODO deal with this
}