//
//  SaveSystem.h
//  Catris
//
//  Created by Maria Goncharuk on 29.10.2026.
//

#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <string>
#include <vector>

struct SavedGame {
    int width = 0;
    int height = 0;
    std::vector<int> cells;
    int pieceX = 0;
    int pieceY = 0;
    int score = 0;
    int lines = 0;
};

class SaveSystem {
public:
    static bool saveToFile(const std::string& path, const SavedGame& game);
    static bool loadFromFile(const std::string& path, SavedGame& out);

    static bool saveHighScore(const std::string& path, int score);
    static int loadHighScore(const std::string& path);
};

#endif

