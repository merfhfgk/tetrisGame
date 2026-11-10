//
//  Board.h
//  
//
//  Created by Maria Goncharuk on 26.10.2026.
//

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Tetromino.h"

class Board {
    
private:
    const int height = 20;
    const int width = 10;
    std::vector<std::vector<int>> grid;
    
public:
    Board();
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    bool isValidMove(int targetX, int targetY) const;
    bool fits (const Tetromino& t) const;
    int getCell(int x, int y) const {return grid[y][x];}
    void setCell(int x, int y, int value) {grid[y][x] = value;}
    void lock(const Tetromino& t);
    int clearLines();
    void clear();
};

#endif
