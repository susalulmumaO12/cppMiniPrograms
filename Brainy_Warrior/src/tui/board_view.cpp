#include "board_view.h"
#include <ncurses.h>
#include "brainy_warrior_tui.h"

void printTile(int value, int r, int c) {
    int pair = 0;
    const char* sym = " ? ";
    switch (value) {
        case 0: pair = SEA_PAIR; sym = "   "; break;
        case 1: pair = LAND_PAIR; sym = " . "; break;
        case 3: pair = WIZARD_TILE_PAIR; sym = " O "; break;
        case 4: pair = WALL_PAIR; sym = " # "; break;
        case 5: pair = TARGET_PAIR; sym = " X "; break;
        case 6: pair = STAR_PAIR; sym = " * "; break;
        case 7: pair = WIZARD_PAIR; sym = " W "; break;
        case 8: pair = WON_PLAYER_PAIR; sym = " @ "; break;
        case 9: pair = PLAYER_PAIR; sym = " P "; break;
        case -1: pair = DEAD_PLAYER_PAIR; sym = " X "; break;
        default: sym = " ? "; break;
    }
    attron(COLOR_PAIR(pair));
    mvprintw(r, c, "%s", sym);
    attroff(COLOR_PAIR(pair));
}

void printBoard(Board board){
    int n = board.getN();
    int m = board.getM();
    int start_r = (LINES - n) / 2;
    int start_c = (COLS - m * 3) / 2;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printTile(board.getTile(i, j).getValue(), start_r + i, start_c + j * 3);
        }
    }
    refresh();
}
