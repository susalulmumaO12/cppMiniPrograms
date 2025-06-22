#ifndef BRAINY_WARRIOR_TUI_H
#define BRAINY_WARRIOR_TUI_H

#include <string>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define BANNER_PAIR 1
#define OPTION_PAIR 2
#define SELECTED_OPTION_PAIR 3
#define GREY_OPTION_PAIR 4

void init_tui();
int main_menu();
std::string ask_name();
void levels_menu();
void stats_menu();
void quit();

void level(const std::string& level);
void stats(const std::string& level);

#endif BRAINY_WARRIOR_TUI_H