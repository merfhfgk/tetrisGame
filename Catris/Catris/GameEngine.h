//
//  GameEngine.h
//  
//
//  Created by Maria Goncharuk on 28.10.2026.
//
#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include <memory>
#include <string>

#include "Board.h"
#include "Tetromino.h"
#include "ScoreManager.h"
#include "SaveSystem.h"

class GameEngine {
private:
    Board board;
    std::unique_ptr<Tetromino> currentPiece;
    ScoreManager scoreManager;
    bool isGameOver;
    bool isPaused;
    bool newRecord;
    std::string savePath;
    std::string recordPath;

    void spawnPiece();
    void lockPiece();
    void endGame();

public:
    explicit GameEngine(std::string savePath = "catris_save.txt",
                        std::string recordPath = "catris_record.txt");

    void start();
    void reset();

    void handleInput(char key);
    void update();

    bool movePiece(int dx, int dy);
    void hardDrop();
    void togglePause();

    bool saveGame() const;
    bool loadGame();

    const Board& getBoard() const {return board;}
    const Tetromino* getCurrentPiece() const {return currentPiece.get();}
    bool getIsGameOver() const {return isGameOver;}
    bool getIsPaused() const {return isPaused;}
    bool getNewRecord() const {return newRecord;}
    const ScoreManager& getScoreManager() const {return scoreManager;}
};

#endif
