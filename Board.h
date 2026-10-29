//
//  Board.h
//  
//
//  Created by Maria Goncharuk on 26.10.2026.
//

#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
    
private:
    const int height = 10;
    const int width = 20;
    std::vector<std::vector<int>> grid;
    
public:
    Board();
    int getWidth() const {return width;}
    int getHeight() const {return height;}
    bool isValidMove(int targetX, int targetY) const;
};

#endif
