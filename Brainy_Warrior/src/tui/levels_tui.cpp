#include "brainy_warrior_tui.h"

#include <fstream>
#include <queue>
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

const std::string instructions = "Type in a letter using either of the following ways: WASD, IJKL, Z for wizard clockwise movement";
const std::string gameOver = "\033[31mGAME OVER! You drowned...\033[0m";
const std::string gameWin = "\033[38;5;226mYOU WIN!\033[0m";

void levels_menu() {
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
