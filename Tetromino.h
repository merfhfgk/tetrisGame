//
//  Tetromino.h
//  
//
//  Created by Maria Goncharuk on 26.10.2026.
//

#ifndef TETROMINO_H
#define TETROMINO_H


class Tetromino {
private:
    int x;
    int y;
public:
    Tetromino(int startX, int startY);
    int getX() const {return x;}
    int getY() const {return y;}
    void moveLeft();
    void moveRight();
    void moveDown();
};

#endif
