//
//  Tetromino.cpp
//  
//
//  Created by Maria Goncharuk on 26.10.2026.
//

#include "Tetromino.h"

Tetromino::Tetromino(TetrominoType type, int startX, int startY) : type(type), x(startX), y(startY), rotation(0) {
    
    switch(type) {
        case TetrominoType::I: cells = {{ {-1, 0}, {0,0}, {1, 0}, {2,0} }}; break;
        case TetrominoType::O: cells = {{ {0, 0}, {1,0}, {0, 1}, {1,1} }}; break;
        case TetrominoType::T: cells = {{ {-1, 0}, {0,0}, {1, 0}, {0,1} }}; break;
        case TetrominoType::S: cells = {{ {0, 0}, {1,0}, {-1, -1}, {0,1} }}; break;
        case TetrominoType::Z: cells = {{ {-1, 0}, {0,0}, {0, 1}, {1,1} }}; break;
        case TetrominoType::J: cells = {{ {-1, 0}, {0, 0}, {1,0}, {-1, 1} }}; break;
        case TetrominoType::L: cells = {{ {-1, 0}, {0,0}, {1, 0}, {1,1} }}; break;
    }
}

std::array<Point, 4> Tetromino::getBlocks() const {
    std::array<Point,4> blocks = cells;
    for (auto& c : blocks) {
        c.x += x;
        c.y += y;
    }
    return blocks;
}

bool Tetromino::occupies(int px, int py) const {
    for (const auto& c : getBlocks()) {
        if (c.x == px && c.y == py) return true;
    }
    return false;
}

void Tetromino::move(int dx, int dy) {
    x += dx;
    y += dy;
}

void Tetromino::rotate() {
    if (type == TetrominoType::O) return;
    for (auto& c : cells) {
        Point old = c;
        c.x = -old.y;
        c.y = old.x;
    }
    rotation = (rotation + 1) % 4;
}
