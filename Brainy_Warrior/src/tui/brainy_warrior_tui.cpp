#include "brainy_warrior_tui.h"

#include <fstream>
#include <queue>
#include <locale.h>
#include <ncurses.h>
#include <curses.h>
#include <menu.h>
#include <nlohmann/json.hpp>
#include "helper_functions.h"

using json = nlohmann::json;


void init_tui() {
    /* Initialize curses */	
    setlocale(LC_ALL, ""); // UTF-8 support
	initscr();
    set_escdelay(25);
	start_color();
    cbreak();
    noecho();
	keypad(stdscr, TRUE); // support arrow keys for stdscr
    curs_set(0);
    apply_theme();
}

void apply_theme() {
    theme current = GameSession::getInstance().currentTheme;

    if (current == high_contrast) {
        init_pair(BANNER_PAIR, COLOR_BLACK, COLOR_WHITE);
        init_pair(OPTION_PAIR, COLOR_WHITE, COLOR_BLACK);
        init_pair(SELECTED_OPTION_PAIR, COLOR_BLACK, COLOR_YELLOW);
        init_pair(GREY_OPTION_PAIR, COLOR_WHITE, COLOR_BLACK);

        init_pair(SEA_PAIR, COLOR_WHITE, COLOR_BLUE);
        init_pair(LAND_PAIR, COLOR_BLACK, COLOR_WHITE);
        init_pair(WIZARD_TILE_PAIR, COLOR_CYAN, COLOR_BLACK);
        init_pair(WALL_PAIR, COLOR_YELLOW, COLOR_BLACK);
        init_pair(TARGET_PAIR, COLOR_RED, COLOR_BLACK);
        init_pair(STAR_PAIR, COLOR_YELLOW, COLOR_BLACK); // High visibility for feedback
        init_pair(WIZARD_PAIR, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(WON_PLAYER_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(PLAYER_PAIR, COLOR_BLUE, COLOR_WHITE);
        init_pair(DEAD_PLAYER_PAIR, COLOR_WHITE, COLOR_RED);
    } else if (current == legacy) {
        init_pair(BANNER_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(OPTION_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(SELECTED_OPTION_PAIR, COLOR_BLACK, COLOR_GREEN);
        init_pair(GREY_OPTION_PAIR, COLOR_WHITE, COLOR_BLACK);

        init_pair(SEA_PAIR, COLOR_BLACK, COLOR_GREEN);
        init_pair(LAND_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(WIZARD_TILE_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(WALL_PAIR, COLOR_WHITE, COLOR_BLACK);
        init_pair(TARGET_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(STAR_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(WIZARD_PAIR, COLOR_BLACK, COLOR_GREEN);
        init_pair(WON_PLAYER_PAIR, COLOR_BLACK, COLOR_GREEN);
        init_pair(PLAYER_PAIR, COLOR_BLACK, COLOR_GREEN);
        init_pair(DEAD_PLAYER_PAIR, COLOR_GREEN, COLOR_BLACK);
    } else { // Classic
        init_pair(BANNER_PAIR, COLOR_CYAN, COLOR_BLACK);
        init_pair(OPTION_PAIR, COLOR_WHITE, COLOR_BLACK);
        init_pair(SELECTED_OPTION_PAIR, COLOR_WHITE, COLOR_RED);
        init_pair(GREY_OPTION_PAIR, COLOR_BLACK, COLOR_WHITE);

        init_pair(SEA_PAIR, COLOR_BLUE, COLOR_CYAN);
        init_pair(LAND_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(WIZARD_TILE_PAIR, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(WALL_PAIR, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(TARGET_PAIR, COLOR_RED, COLOR_BLACK);
        init_pair(STAR_PAIR, COLOR_YELLOW, COLOR_BLACK);
        init_pair(WIZARD_PAIR, COLOR_MAGENTA, COLOR_WHITE);
        init_pair(WON_PLAYER_PAIR, COLOR_YELLOW, COLOR_GREEN);
        init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_BLUE);
        init_pair(DEAD_PLAYER_PAIR, COLOR_RED, COLOR_WHITE);
    }
}

int main_menu() {
    debug_log("Entering main_menu");
    clear();
    const char *choices[] = {"Play Levels", "Computer Play", "View Stats", "Settings", "About", "Quit Game"};
    
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
    for(int i = 0; i < n_choices; ++i) {
        items[i] = new_item(choices[i], "");
    }
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
    debug_log("main_menu selected: " + std::to_string(choice));

    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < n_choices; ++i)
        free_item(items[i]);
    free(items);
    delwin(menuWin);

    return choice;
}

int choose_algorithm_menu() {
    clear();
    const char *choices[] = {"BFS", "DFS", "UCS", "Hill Climbing", "A*", "< Back >"};
    
    ITEM **items;
    int c;				
	MENU *menu;
    WINDOW *menuWin;
    int n_choices;
    int winHeight = 12, winWidth = 40;
    int startx = (COLS - winWidth) / 2;
    int starty = (LINES - winHeight) / 2;
    const char* title = " CHOOSE ALGORITHM ";

    n_choices = ARRAY_SIZE(choices);
    items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i) {
        items[i] = new_item(choices[i], "");
    }
    items[n_choices] = (ITEM *)NULL;
	menu = new_menu((ITEM **)items);
    menuWin = newwin(winHeight, winWidth, starty, startx);
    set_menu_fore(menu, COLOR_PAIR(SELECTED_OPTION_PAIR));
    set_menu_back(menu, COLOR_PAIR(OPTION_PAIR));
    set_menu_mark(menu, " > ");
    set_menu_win(menu, menuWin);
    set_menu_sub(menu, derwin(menuWin, 8, 38, 2, 1));

    keypad(menuWin, TRUE);
    box(menuWin, 0, 0);
	mvwprintw(menuWin, 0, (40 - strlen(title)) / 2, "%s", title);

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
    free(items);
    delwin(menuWin);

    if (choice == 5) return -1; // Back
    return choice;
}

std::string ask_name() {
    std::string name;
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

    return name;
}

void settings_menu() {
    clear();
    const char *choices[] = {"Theme: Classic", "Theme: High Contrast", "Theme: Legacy", "< Back >"};
    
    ITEM **items;
    int c;				
	MENU *menu;
    WINDOW *menuWin;
    int n_choices;
    int winHeight = 10, winWidth = 40;
    int startx = (COLS - winWidth) / 2;
    int starty = (LINES - winHeight) / 2;
    const char* title = "   SETTINGS   ";

    n_choices = ARRAY_SIZE(choices);
    items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(int i = 0; i < n_choices; ++i) {
        items[i] = new_item(choices[i], "");
    }
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
    
    if (choice == 0) GameSession::getInstance().currentTheme = classic;
    else if (choice == 1) GameSession::getInstance().currentTheme = high_contrast;
    else if (choice == 2) GameSession::getInstance().currentTheme = legacy;

    apply_theme();

    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < n_choices; ++i)
        free_item(items[i]);
    free(items);
    delwin(menuWin);
}

void about_menu() {
    clear();
    curs_set(0);
    int row, col;
    getmaxyx(stdscr, row, col);

    const char* title = " ABOUT BRAINY WARRIOR ";
    std::string text[] = {
        "Brainy Warrior is a strategic puzzle game.",
        "Goal: Defeat all enemies (X) by sliding.",
        "Beware: If you slide without hitting a target",
        "or a wall, you'll fall into the sea!",
        "",
        "Special Tiles:",
        " O - Wizard positions (rotate with Z)",
        " W - Active Wizard (acts as a stop)",
        "",
        "Programmer: susalulmumaO12",
        "",
        "Press any key to return..."
    };

    attron(A_BOLD | COLOR_PAIR(BANNER_PAIR));
    mvprintw(row / 4 - 2, (col - strlen(title)) / 2, "%s", title);
    attroff(A_BOLD | COLOR_PAIR(BANNER_PAIR));

    for (size_t i = 0; i < 12; ++i) {
        mvprintw(row / 4 + i, (col - text[i].length()) / 2, "%s", text[i].c_str());
    }

    refresh();
    getch();
}

void quit() {
    clear();
    int row, col;
    getmaxyx(stdscr, row, col);
    const char* msg = "Goodbye! Happy playing!";
    attron(A_BOLD | COLOR_PAIR(STAR_PAIR));
    mvprintw(row / 2, (col - strlen(msg)) / 2, "%s", msg);
    attroff(A_BOLD | COLOR_PAIR(STAR_PAIR));
    refresh();
    napms(1000); // Wait for 1 second
    endwin();
}
