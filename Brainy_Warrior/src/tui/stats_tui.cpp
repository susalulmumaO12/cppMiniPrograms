#include "brainy_warrior_tui.h"

#include <fstream>
#include <queue>
#include <vector>
#include <locale.h>
#include <ncurses.h>
#include <curses.h>
#include <menu.h>
#include <nlohmann/json.hpp>
#include "algorithms.h"
#include "tile.h"
#include "board.h"
#include "helper_functions.h"
#include "moves.h"

using json = nlohmann::json;

void stats_menu() {
    clear();
    refresh();

    json statsJson;
    std::ifstream statsFile("../stats.json");
    if (statsFile.is_open()) {
        statsFile >> statsJson;
        statsFile.close();
    }

    std::vector<std::string> labels;
    std::vector<std::string> levelKeys;

    for (const auto& level : statsJson["levels"].items()) {
        std::string label = level.key() + " stats";
        levelKeys.push_back(level.key());
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
    for(int i = 0; i < n_choices; ++i) {
        items[i] = new_item(choices[i], "");
    }
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
    
    if (focusOnBack) {
        return;
    } else {
        choice = item_index(current_item(menu));
    }
    if (choice >= 0 && choice < (int)levelKeys.size()) {
        std::string levelKey = levelKeys[choice];
        stats(levelKey);
    }

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

}

void stats(const std::string& level) {
    std::vector<std::pair<int, std::pair<int, std::pair<int, std::string>>>> score_pairs = fetchStats(level);
    std::string currentPlayer = GameSession::getInstance().playerName;

    clear();
    curs_set(0);
    int row, col;
    getmaxyx(stdscr, row, col);

    std::string header = "Stats for " + level + " (Player: " + currentPlayer + ")";
    int starty = row / 4;
    int startx = (col - 40) / 2;

    attron(A_BOLD | COLOR_PAIR(BANNER_PAIR));
    mvprintw(starty - 2, (col - header.size()) / 2, "%s", header.c_str());
    attroff(A_BOLD | COLOR_PAIR(BANNER_PAIR));

    attron(A_UNDERLINE | COLOR_PAIR(OPTION_PAIR));
    mvprintw(starty, startx, "%-12s | %-5s | %-5s | %-5s", "Name", "Score", "Tries", "Wins");
    attroff(A_UNDERLINE | COLOR_PAIR(OPTION_PAIR));

    int i = 1;
    bool found = false;
    for (const auto& player : score_pairs) {
        std::string name   = player.second.second.second;
        if (name != currentPlayer) continue; // Filter by current player
        
        found = true;
        int score          = player.first;
        int tries          = player.second.first;
        int wins           = player.second.second.first;

        attron(COLOR_PAIR(OPTION_PAIR));
        mvprintw(starty + i, startx, "%-12s | %-5d | %-5d | %-5d", name.c_str(), score, tries, wins);
        attroff(COLOR_PAIR(OPTION_PAIR));
        i++;
    }

    if (!found) {
        mvprintw(starty + 2, (col - 25) / 2, "No stats found for you.");
    }

    attron(A_DIM);
    mvprintw(row - 2, (col - 35) / 2, "Press any key to return...");
    attroff(A_DIM);

    refresh();
    getch();
    clear();
    refresh();
}
