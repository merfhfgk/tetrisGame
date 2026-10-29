//
//  GameEngine.h
//  
//
//  Created by Maria Goncharuk on 28.10.2026.
//
#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "Board.h"
#include "Tetromino.h"

class GameEngine{
private:
    Board board;
    Tetromino* currentPiece;
    bool isGameOver;
public:
    GameEngine();
    ~GameEngine();
    void start();
    void handleInput(char key);
    void update();
    
    const Board& getBoard() const {return board;}
    const Tetromino* getCurrentPiece() const {return currentPiece;}
    bool getIsGameOver() const {return isGameOver;}
};

#endif 
