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
#include <vector>
#include <random>

#include "Board.h"
#include "Tetromino.h"
#include "ScoreManager.h"
#include "SaveSystem.h"

enum class Action {Left, Right, SoftDrop, HardDrop, Rotate, Pause, Save, Load, Quit };

enum class GameState {Menu, Playing, Paused, GameOver};

class GameEngine {
private:
    Board board;
    std::unique_ptr<Tetromino> currentPiece;
    TetrominoType nextType = TetrominoType::I;
    ScoreManager scoreManager;
    GameState state = GameState::Menu;
    bool newRecord = false;
    float fallTime = 0.0f;
    float lockTimer = 0.0f;
    static constexpr float lockDelay = 0.5f;
    static constexpr float maxDt = 0.25f;
    std::mt19937 rng{std::random_device{}()};
    std::vector<TetrominoType> bag;
    std::string savePath;
    std::string recordPath;

    TetrominoType drawFromBag();
    bool canMoveDown() const;
    void spawnPiece();
    void lockPiece();
    void endGame();

public:
    explicit GameEngine(std::string savePath = "catris_save.txt",
                        std::string recordPath = "catris_record.txt");

    void start();
    void reset();

    void handleInput(Action action);
    void update(float dt);

    bool movePiece(int dx, int dy);
    void hardDrop();
    void togglePause();
    bool rotatePiece();

    bool saveGame() const;
    bool loadGame();

    const Board& getBoard() const {return board;}
    const Tetromino* getCurrentPiece() const {return currentPiece.get();}
    bool getIsGameOver() const {return state == GameState::GameOver;}
    bool getIsPaused() const {return state == GameState::Paused;}
    bool getNewRecord() const {return newRecord;}
    const ScoreManager& getScoreManager() const {return scoreManager;}
    TetrominoType getNextType() const {return nextType;}
    GameState getState() const {return state;}
    float getFallInterval() const;
};

#endif
