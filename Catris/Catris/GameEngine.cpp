//
//  GameEngine.cpp
//  
//
//  Created by Maria Goncharuk on 28.10.2026.
//
#include "GameEngine.h"

GameEngine::GameEngine() : currentPiece(nullptr), isGameOver(false) {}

GameEngine::~GameEngine(){
    delete currentPiece;
}

void GameEngine::start(){
    delete currentPiece;
    currentPiece = new Tetromino(4, 0);
    isGameOver = false;
}

void GameEngine::handleInput(char key) {
    if (!currentPiece || isGameOver) return;
    
    if (key == 'a' && board.isValidMove(currentPiece->getX() - 1, currentPiece->getY())) {
            currentPiece->moveLeft();
    }
    else if (key == 'd' && board.isValidMove(currentPiece->getX() + 1, currentPiece->getY())) {
            currentPiece->moveRight();
    }
    else if (key == 's' && board.isValidMove(currentPiece->getX(), currentPiece->getY() + 1)) {
            currentPiece->moveDown();
    }
    else if (key == 'q') {
            isGameOver = true;
    }
}

void GameEngine::update() {
    // update logic
}
