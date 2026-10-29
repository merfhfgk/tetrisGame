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
    return true;
}
