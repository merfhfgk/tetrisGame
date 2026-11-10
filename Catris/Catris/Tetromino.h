//
//  Tetromino.h
//  
//
//  Created by Maria Goncharuk on 26.10.2026.
//

#ifndef TETROMINO_H
#define TETROMINO_H
#include <array>

enum class TetrominoType { I, O, T, S, Z, J, L};

struct Point {
    int x; int y;
};

class Tetromino {
private:
    TetrominoType type;
    int x;
    int y;
    int rotation;
    std::array<Point, 4> cells;
public:
    Tetromino(TetrominoType type, int startX, int startY);
    TetrominoType getType() const {return type;}
    int getX() const {return x;}
    int getY() const {return y;}
    int getRotation() const {return rotation;}
    std::array<Point, 4> getBlocks() const;
    bool occupies(int px, int py) const;
    void move(int dx, int dy);
    void rotate();
};



#endif
