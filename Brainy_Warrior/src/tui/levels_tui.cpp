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

std::string choose_level_menu() {
    debug_log("Entering choose_level_menu");
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
        bool played = statsJson["levels"].contains(levelName) && statsJson["levels"][levelName].contains(GameSession::getInstance().playerName);

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

    debug_log("levels_menu loop finished, char: " + std::to_string(c));

    int choice = -1;
    if (!focusOnBack) {
        choice = item_index(current_item(menu));
    }
    debug_log("levels_menu choice: " + std::to_string(choice) + " focusOnBack: " + std::to_string(focusOnBack));

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
        return "";
    }
    if (choice >= 0 && choice < (int)levelKeys.size()) {
        return levelKeys[choice];
    }
    return "";
}

void levels_menu() {
    std::string levelKey = choose_level_menu();
    if (!levelKey.empty()) {
        clear();
        level(levelKey);
    }
}


void level(const std::string& level) {
    Board board = set_game_board(level);
    
    auto& session = GameSession::getInstance();
    session.wizardTiles = getWizardTiles(board);
    
    clear();
    printBoard(board);
    mvprintw(LINES - 1, (COLS - instructions.size()) / 2, "%s", instructions.c_str());
    refresh();

    auto state = std::make_shared<Node_State>(board, nullptr, 0, 0);
    std::string feedback;
    std::queue<std::shared_ptr<Node_State>> q;
    q.push(state);
    
    while(!q.empty()){
        Tile& player = board.getPlayerTile();
        std::shared_ptr<Node_State> current = q.front();
        q.pop();
        
        debug_log("In level loop, player at " + std::to_string(player.getRow()) + "," + std::to_string(player.getCol()));

        // WIN CASE
        if(board.win()){
            clear();
            printBoard(board);
            attron(A_BOLD | COLOR_PAIR(WON_PLAYER_PAIR));
            mvprintw(LINES - 4, (COLS - 10) / 2, " YOU WIN! ");
            attroff(A_BOLD | COLOR_PAIR(WON_PLAYER_PAIR));
            
            updateStats(session.playerName, level, true);
            mvprintw(LINES - 2, (COLS - 20) / 2, "Print path? (y/n)");
            mvprintw(LINES - 1, (COLS - instructions.size()) / 2, "%s", instructions.c_str());
            refresh();
            int yn1 = getch();
            if(tolower(yn1) == 'y') {
                clear();
                printBoard(getPath(current));
                attron(A_BOLD | COLOR_PAIR(STAR_PAIR));
                mvprintw(LINES - 3, (COLS - 12) / 2, " PATH FOUND ");
                attroff(A_BOLD | COLOR_PAIR(STAR_PAIR));
                mvprintw(LINES - 1, (COLS - 25) / 2, "Press any key to exit...");
                refresh();
                getch();
            }
            return;
        }
        
        // LOSE CASE
        if(player.getValue() == -1){
            clear();
            printBoard(board);
            attron(A_BOLD | COLOR_PAIR(DEAD_PLAYER_PAIR));
            mvprintw(LINES - 3, (COLS - 25) / 2, " GAME OVER! You drowned... ");
            attroff(A_BOLD | COLOR_PAIR(DEAD_PLAYER_PAIR));
            
            updateStats(session.playerName, level, false);
            mvprintw(LINES - 1, (COLS - 30) / 2, "Press any key to return to menu...");
            refresh();
            getch();
            return;
        }

        int m = getch();
        debug_log("Key pressed in level: " + std::to_string(m));
        if (m == 27) return; // ESC to quit level
        
        char moveChar = 0;
        switch(m) {
            case KEY_UP:    moveChar = 'w'; break;
            case KEY_DOWN:  moveChar = 's'; break;
            case KEY_LEFT:  moveChar = 'a'; break;
            case KEY_RIGHT: moveChar = 'd'; break;
            default:        moveChar = (char)m; break;
        }

        Board nextBoard = move(board, moveChar);
        
        if (stringBoard(nextBoard) == stringBoard(board)) {
            feedback = "Invalid Move!";
        } else {
            feedback = "";
            if (tolower(moveChar) == 'z') feedback = "Wizard Rotated!";
            board = nextBoard;
        }

        auto movedBoard = std::make_shared<Node_State>(board, current, 0, 0);
        q.push(movedBoard);
        
        clear();
        printBoard(board);
        if (!feedback.empty()) {
            attron(A_BOLD | COLOR_PAIR(STAR_PAIR));
            mvprintw((LINES + board.getN()) / 2 + 2, (COLS - feedback.size()) / 2, "%s", feedback.c_str());
            attroff(A_BOLD | COLOR_PAIR(STAR_PAIR));
        }
        mvprintw(LINES - 1, (COLS - instructions.size()) / 2, "%s", instructions.c_str());
        refresh();
    }
}
