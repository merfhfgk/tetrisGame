//
//  SaveSystem.cpp
//  Catris
//
//  Created by Maria Goncharuk on 29.10.2026.
//

#include "SaveSystem.h"
#include <fstream>

namespace {
const char* MAGIC = "CATRIS2";
}

bool SaveSystem::saveToFile(const std::string& path, const SavedGame& g) {
    std::ofstream f(path);
    if (!f) return false;
    f << MAGIC << '\n'
      << g.width << ' ' << g.height << '\n'
      << g.score << ' ' << g.lines << '\n'
      << g.pieceX << ' ' << g.pieceY << '\n';
    for (int y = 0; y < g.height; ++y) {
        for (int x = 0; x < g.width; ++x) {
            f << g.cells[y * g.width + x] << (x + 1 < g.width ? ' ' : '\n');
        }
    }
    return static_cast<bool>(f);
}

bool SaveSystem::loadFromFile(const std::string& path, SavedGame& out) {
    std::ifstream f(path);
    if (!f) return false;

    std::string magic;
    f >> magic;
    if (magic != MAGIC) return false;

    SavedGame g;
    f >> g.width >> g.height >> g.score >> g.lines >> g.pieceX >> g.pieceY;
    if (!f || g.width <= 0 || g.height <= 0 || g.width > 1000 || g.height > 1000) return false;
    if (g.score < 0 || g.lines < 0) return false;

    g.cells.resize(g.width * g.height);
    for (int& c : g.cells) {
        f >> c;
        if (!f || (c != 0 && c != 1)) return false;
    }
    out = g;
    return true;
}

bool SaveSystem::saveHighScore(const std::string& path, int score) {
    std::ofstream f(path);
    if (!f) return false;
    f << score << '\n';
    return static_cast<bool>(f);
}

int SaveSystem::loadHighScore(const std::string& path) {
    std::ifstream f(path);
    int value = 0;
    if (f >> value && value > 0) return value;
    return 0;
}
