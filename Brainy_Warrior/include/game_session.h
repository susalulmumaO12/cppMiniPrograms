#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include <string>
#include <vector>
#include "tile.h"

enum dist { man, euc, mink, cheb };
enum theme { classic, high_contrast, legacy };

struct circularq {
    std::vector<Tile> tiles;
    int front = 0;

    void push(Tile t){
        tiles.emplace_back(t);
    }

    Tile nextTile(){
        if (tiles.empty()) return Tile();
        front = (front + 1) % tiles.size();
        return tiles[front];
    }
};

class GameSession {
public:
    bool wizMoved = false;
    circularq wizardTiles;
    Tile currentWizard;
    int score = 0;
    std::string playerName;
    dist distanceType = man;
    theme currentTheme = classic;

    static GameSession& getInstance() {
        static GameSession instance;
        return instance;
    }

private:
    GameSession() = default;
};

#endif
