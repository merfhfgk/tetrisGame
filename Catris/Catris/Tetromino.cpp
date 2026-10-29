//
//  Tetromino.cpp
//  
//
//  Created by Maria Goncharuk on 26.10.2026.
//

#include "Tetromino.h"

Tetromino::Tetromino(int startX, int startY) : x(startX), y(startY) {}

void Tetromino::moveLeft() {x--;}
void Tetromino::moveRight() {x++;}
void Tetromino::moveDown() {y++;}

