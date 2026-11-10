//
//  ConsoleReader.h
//  
//
//  Created by Maria Goncharuk on 29.10.2026.
//

#ifndef CONSOLE_RENDER_H
#define CONSOLE_RENDER_H

#include "GameEngine.h"
#include <cctype>
#include <iostream>
#include <cstdlib>

class ConsoleRenderer {
public:
    
    void run(GameEngine& engine) {
        engine.start();

        char input;
        while (!engine.getIsGameOver()) {
            render(engine);
            if (!(std::cin >> input)) break;

            Action action;
            if (keyToAction(input, action)) {
                engine.handleInput(action);
            }
            engine.update(TURN_TIME);
        }

        std::cout << "Game Over" << std::endl;
    }

private:
    static constexpr float TURN_TIME = 0.25f;
    
    bool keyToAction(char key, Action& action) const {
        switch (std::tolower(static_cast<unsigned char>(key))) {
            case 'a': action = Action::Left; return true;
            case 'd': action = Action::Right; return true;
            case 's': action = Action::SoftDrop; return true;
            case 'w': action = Action::Rotate; return true;
            case 'x': action = Action::HardDrop; return true;
            case 'p': action = Action::Pause; return true;
            case 'k': action = Action::Save; return true;
            case 'l': action = Action::Load; return true;
            case 'q': action = Action::Quit; return true;
            default: return false;
        }
    }
    
    static char typeLetter(TetrominoType t) {
        return "IOTSZJL"[static_cast<int>(t)];
    }
    
    void render(const GameEngine& engine) const {
        system("clear");

        const Board& board = engine.getBoard();
        const Tetromino* piece = engine.getCurrentPiece();
        const ScoreManager& sm = engine.getScoreManager();

        std::cout << "CATRIS   Score: " << sm.getScore()
                  << "   Record: " << sm.getHighScore()
                  << "   Lines: " << sm.getLines()
                  << "   Next: " << typeLetter(engine.getNextType())
                  << (engine.getIsPaused() ? "   [PAUSED]" : "") << std::endl;

        for (int y = 0; y < board.getHeight(); ++y) {
            for (int x = 0; x < board.getWidth(); ++x) {
                if (piece != nullptr && piece->occupies(x, y)) {
                    std::cout << "[#]";
                }
                else if (board.getCell(x, y) != 0) {
                    std::cout << "[=]";
                }
                else {
                    std::cout << " . ";
                }
            }
            std::cout << std::endl;
        }

        std::cout << "Controls: a/d (move), s (down), w (rotate), x (drop), p (pause), k (save), l (load), q (quit)" << std::endl;
        std::cout << "Enter command and press Return: ";
    }
};

#endif
