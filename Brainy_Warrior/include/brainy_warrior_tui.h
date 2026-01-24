#ifndef BRAINY_WARRIOR_TUI_H
#define BRAINY_WARRIOR_TUI_H

#include <string>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define BANNER_PAIR 1
#define OPTION_PAIR 2
#define SELECTED_OPTION_PAIR 3
#define GREY_OPTION_PAIR 4

// Tile colors
#define SEA_PAIR 5
#define LAND_PAIR 6
#define WIZARD_TILE_PAIR 7
#define WALL_PAIR 8
#define TARGET_PAIR 9
#define STAR_PAIR 10
#define WIZARD_PAIR 11
#define WON_PLAYER_PAIR 12
#define PLAYER_PAIR 13
#define DEAD_PLAYER_PAIR 14

void init_tui();
int main_menu();
std::string ask_name();
std::string choose_level_menu();
int choose_algorithm_menu();
void levels_menu();
void computer_play();
void stats_menu();
void quit();

void level(const std::string& level);
void stats(const std::string& level);

#endif BRAINY_WARRIOR_TUI_H