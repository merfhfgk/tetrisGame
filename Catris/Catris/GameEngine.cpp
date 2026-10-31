//
//  GameEngine.cpp
//  
//
//  Created by Maria Goncharuk on 28.10.2026.
//
#include "GameEngine.h"
#include <cctype>

GameEngine::GameEngine(std::string savePath, std::string recordPath)
    : currentPiece(nullptr), isGameOver(false), isPaused(false), newRecord(false),
      savePath(std::move(savePath)), recordPath(std::move(recordPath)) {
    scoreManager.setHighScore(SaveSystem::loadHighScore(this->recordPath));
}

void GameEngine::start() {
    reset();
}

void GameEngine::reset() {
    board.clear();
    scoreManager.reset();
    isGameOver = false;
    isPaused = false;
    newRecord = false;
    spawnPiece();
}

void GameEngine::handleInput(char key) {
    if (!currentPiece || isGameOver) return;

    key = static_cast<char>(std::tolower(static_cast<unsigned char>(key)));

    if (key == 'q') { endGame(); return; }
    if (key == 'p') { togglePause(); return; }
    if (key == 'k') { saveGame(); return; }
    if (key == 'l') { loadGame(); return; }
    if (isPaused) return;

    switch (key) {
        case 'a': movePiece(-1, 0); break;
        case 'd': movePiece(1, 0); break;
        case 's': if (movePiece(0, 1)) scoreManager.addPoints(1); break;
        case 'w': hardDrop(); break;
        default: break;
    }
}

bool GameEngine::movePiece(int dx, int dy) {
    if (!currentPiece || isGameOver || isPaused) return false;

    if (!board.isValidMove(currentPiece->getX() + dx, currentPiece->getY() + dy)) return false;

    if (dx < 0) currentPiece->moveLeft();
    else if (dx > 0) currentPiece->moveRight();
    if (dy > 0) currentPiece->moveDown();
    return true;
}

void GameEngine::hardDrop() {
    if (!currentPiece || isGameOver || isPaused) return;
    int cells = 0;
    while (movePiece(0, 1)) ++cells;
    scoreManager.addPoints(2 * cells);
    lockPiece();
}

void GameEngine::togglePause() {
    if (!currentPiece || isGameOver) return;
    isPaused = !isPaused;
}

void GameEngine::update() {
    if (!currentPiece || isGameOver || isPaused) return;

    if (!movePiece(0, 1)) {
        lockPiece();
    }
}

void GameEngine::lockPiece() {
    board.setCell(currentPiece->getX(), currentPiece->getY(), 1);
    scoreManager.addPoints(10);
    scoreManager.addLineClear(board.clearLines());
    spawnPiece();
}

void GameEngine::spawnPiece() {
    const int spawnX = board.getWidth() / 2;
    currentPiece = std::make_unique<Tetromino>(spawnX, 0);
    if (!board.isValidMove(spawnX, 0)) {
        endGame();
    }
}

void GameEngine::endGame() {
    isGameOver = true;
    if (scoreManager.isNewRecord()) {
        scoreManager.setHighScore(scoreManager.getScore());
        SaveSystem::saveHighScore(recordPath, scoreManager.getScore());
        newRecord = true;
    }
}

bool GameEngine::saveGame() const {
    if (!currentPiece || isGameOver) return false;

    SavedGame g;
    g.width = board.getWidth();
    g.height = board.getHeight();
    for (int y = 0; y < g.height; ++y)
        for (int x = 0; x < g.width; ++x) g.cells.push_back(board.getCell(x, y));
    g.pieceX = currentPiece->getX();
    g.pieceY = currentPiece->getY();
    g.score = scoreManager.getScore();
    g.lines = scoreManager.getLines();
    return SaveSystem::saveToFile(savePath, g);
}

bool GameEngine::loadGame() {
    SavedGame g;
    if (!SaveSystem::loadFromFile(savePath, g)) return false;
    if (g.width != board.getWidth() || g.height != board.getHeight()) return false;

    Board check;
    for (int y = 0; y < g.height; ++y)
        for (int x = 0; x < g.width; ++x)
            check.setCell(x, y, g.cells[y * g.width + x]);
    if (!check.isValidMove(g.pieceX, g.pieceY)) return false;

    for (int y = 0; y < g.height; ++y)
        for (int x = 0; x < g.width; ++x)
            board.setCell(x, y, g.cells[y * g.width + x]);

    currentPiece = std::make_unique<Tetromino>(g.pieceX, g.pieceY);
    scoreManager.restore(g.score, g.lines);
    isGameOver = false;
    isPaused = false;
    newRecord = false;
    return true;
}
