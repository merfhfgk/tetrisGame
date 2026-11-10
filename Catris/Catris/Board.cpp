//
//  Board.cpp
//  
//
//  Created by Maria Goncharuk on 26.10.2026.
//

#include "Board.h"

Board::Board(){
    grid.resize(height, std::vector<int>(width, 0));
}

bool Board::isValidMove(int targetX, int targetY) const {
    if (targetX < 0 || targetX >= width || targetY < 0 || targetY >= height ){
        return false;
    }
    if (grid[targetY][targetX] != 0) {
        return false;
    }
    return true;
}

bool Board::fits(const Tetromino& t) const {
    for (const auto& c : t.getBlocks()) {
        if (!isValidMove(c.x, c.y)) return false;
    }
    return true;
}

void Board::lock(const Tetromino& t) {
    const int value = static_cast<int>(t.getType()) + 1;
    for (const auto& c : t.getBlocks()) {
        if (c.x >= 0 && c.x < width && c.y >= 0 && c.y < height){
            grid[c.y][c.x] = value;
        }
    }
}

void Board::clear(){
    for (auto& row : grid) row.assign(width, 0);
}

int Board::clearLines(){
    int cleared = 0;
    int writeRow = height - 1;
    for (int readRow = height - 1; readRow >= 0; --readRow) {
        bool full = true;
        for (int x = 0; x < width; ++x) {
            if (grid[readRow][x] == 0) {full = false; break;}
        }
        if (full) { ++cleared; continue;}
        if (writeRow != readRow) grid[writeRow] = grid[readRow];
        --writeRow;
    }
    for (int y = writeRow; y >= 0; --y) grid[y].assign(width, 0);
    return cleared;
}
