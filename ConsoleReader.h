//
//  ConsoleReader.h
//  
//
//  Created by Maria Goncharuk on 29.10.2026.
//

#ifndef CONSOLE_RENDER_H
#define CONSOLE_RENDER_H

#include "GameEngine.h"
#include <iostream>
#include <cstdlib>

class ConsoleRenderer {
public:
    
    void run(GameEngine& engine) {
        engine.start();

        char input;
        while (!engine.getIsGameOver()) {
            render(engine);
            std::cin >> input;
            engine.handleInput(input);
            engine.update();
        }

        std::cout << "Game Over" << std::endl;
    }

private:
    void render(const GameEngine& engine) const {
        system("clear");

        const Board& board = engine.getBoard();
        const Tetromino* piece = engine.getCurrentPiece();

        std::cout << "CATRIS test" << std::endl;

        for (int y = 0; y < board.getHeight(); ++y) {
            for (int x = 0; x < board.getWidth(); ++x) {
                if (piece != nullptr && piece->getX() == x && piece->getY() == y) {
                    std::cout << "[#]";
                }
                else {
                    std::cout << " . ";
                }
            }
            std::cout << std::endl;
        }
        
        std::cout << "Controls: a (left), d (right), s (down), q (quit)" << std::endl;
        std::cout << "Enter command and press Return: ";
    }
};

#endif
