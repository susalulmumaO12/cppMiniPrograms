#include "brainy_warrior_tui.h"

#include <fstream>
#include <queue>
#include <locale.h>
#include <ncurses.h>
#include <curses.h>
#include <menu.h>
#include <nlohmann/json.hpp>

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
	
    init_pair(BANNER_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(OPTION_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SELECTED_OPTION_PAIR, COLOR_WHITE, COLOR_RED);
    init_pair(GREY_OPTION_PAIR, COLOR_BLACK, COLOR_WHITE); //TODO: look into color coding in various terminal emulators
}

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