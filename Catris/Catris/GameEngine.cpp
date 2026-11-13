//
//  GameEngine.cpp
//  
//
//  Created by Maria Goncharuk on 28.10.2026.
//
#include "GameEngine.h"
#include <algorithm>
#include "CatStrategies.h"
#include <cstdlib>

class PawPushStrategy : public ICatStrategy {
public:
    void apply(GameEngine& engine) override {
        int dir = (std::rand() % 2 == 0) ? -5 : 5;
        engine.movePiece(dir, 0);
    }
    std::string getMessage() const override { return "Paw Push!"; }
};

class SleepOnKeyboardStrategy : public ICatStrategy {
public:
    void apply(GameEngine& engine) override {
        engine.blockInput(5.0f);
    }
    std::string getMessage() const override { return "Cat on Keyboard!"; }
};

GameEngine::GameEngine(std::string savePath, std::string recordPath) : savePath(std::move(savePath)), recordPath(std::move(recordPath)) {
        scoreManager.setHighScore(SaveSystem::loadHighScore(this->recordPath));
}

void GameEngine::start() {
    reset();
}

void GameEngine::reset() {
    board.clear();
    scoreManager.reset();
    newRecord = false;
    bag.clear();
    nextType = drawFromBag();
    state = GameState::Playing;
    spawnPiece();
    catTimer = 60.0f;
    catEffectDuration = 0.0f;
}

TetrominoType GameEngine::drawFromBag() {
    if (bag.empty()) {
        for (int i = 0; i < 7; ++i) bag.push_back(static_cast<TetrominoType>(i));
        std::shuffle(bag.begin(), bag.end(), rng);
    }
    TetrominoType t = bag.back();
    bag.pop_back();
    return t;
}

void GameEngine::spawnPiece(){
    currentPiece = std::make_unique<Tetromino>(nextType, board.getWidth()/2 - 1, 0);
    nextType = drawFromBag();
    fallTime = 0.0f;
    lockTimer = 0.0f;
    if (!board.fits(*currentPiece)) {
        endGame();
    }
}

float GameEngine::getFallInterval() const {
    return std::max(0.1f, 0.8f - (scoreManager.getLevel() - 1) * 0.07f);
}

void GameEngine::handleInput(Action action) {
    if (state != GameState::Playing && state != GameState::Paused) return;

    switch (action) {
        case Action::Quit:  endGame(); return;
        case Action::Pause: togglePause(); return;
        case Action::Save:  saveGame(); return;
        case Action::Load:  loadGame(); return;
        default: break;
    }
    
    if (isInputBlocked()) {
        if (action == Action::Left || action == Action::Right ||
            action == Action::SoftDrop || action == Action::HardDrop ||
            action == Action::Rotate) {
            return;
        }
    }

    if (state == GameState::Paused) return;

    switch (action) {
        case Action::Left:     movePiece(-1, 0); break;
        case Action::Right:    movePiece(1, 0); break;
        case Action::SoftDrop: if (movePiece(0, 1)) scoreManager.addPoints(1); break;
        case Action::HardDrop: hardDrop(); break;
        case Action::Rotate:   rotatePiece(); break;
        default: break;
    }
}

bool GameEngine::movePiece(int dx, int dy) {
    if (!currentPiece || state != GameState::Playing) return false;

    Tetromino candidate = *currentPiece;
    candidate.move(dx, dy);
    if (!board.fits(candidate)) return false;

    *currentPiece = candidate;
    lockTimer = 0.0f;
    return true;
}

bool GameEngine::rotatePiece() {
    if (state != GameState::Playing || !currentPiece) return false;

    Tetromino rotated = *currentPiece;
    rotated.rotate();

    static const int kicks[][2] = {{0, 0}, {-1, 0}, {1, 0}, {-2, 0}, {2, 0}, {0, 1}, {-1, 1}, {1, 1}};
    for (const auto& k : kicks) {
        Tetromino candidate = rotated;
        candidate.move(k[0], k[1]);
        if (board.fits(candidate)) {
            *currentPiece = candidate;
            lockTimer = 0.0f;
            return true;
        }
    }
    return false;
}

void GameEngine::hardDrop() {
    if (!currentPiece || state != GameState::Playing) return;
    int cells = 0;
    while (movePiece(0, 1)) ++cells;
    scoreManager.addPoints(2 * cells);
    lockPiece();
}

void GameEngine::togglePause() {
    if (state == GameState::Playing) state = GameState::Paused;
    else if (state == GameState::Paused) state = GameState::Playing;
}

bool GameEngine::canMoveDown() const {
    Tetromino candidate = *currentPiece;
    candidate.move(0, 1);
    return board.fits(candidate);
}

void GameEngine::update(float dt) {
    if (!currentPiece || state != GameState::Playing) return;
    
    dt = std::min(dt, maxDt);
    if (catEffectDuration > 0.0f) {
        catEffectDuration -= dt;
    }

    catTimer -= dt;
    if (catTimer <= 0.0f) {
        triggerCatEvent();
        catTimer = 60.0f;
    }
    
    if (inputBlockTimer > 0.0f) {
        inputBlockTimer -= dt;
    }
        
    if (canMoveDown()) {
        lockTimer = 0.0f;
        fallTime += dt;
        if (fallTime >= getFallInterval()) {
            fallTime -= getFallInterval();
            movePiece(0, 1);
        }
    } else {
        fallTime = 0.0f;
        lockTimer += dt;
        if (lockTimer >= lockDelay) lockPiece();
    }
}

void GameEngine::lockPiece() {
    board.lock(*currentPiece);
    scoreManager.addPoints(10);
    scoreManager.addLineClear(board.clearLines());
    spawnPiece();
}

void GameEngine::endGame() {
    state = GameState::GameOver;
    if (scoreManager.isNewRecord()) {
        scoreManager.setHighScore(scoreManager.getScore());
        SaveSystem::saveHighScore(recordPath, scoreManager.getScore());
        newRecord = true;
    }
}

bool GameEngine::saveGame() const {
    if ((state != GameState::Playing && state != GameState::Paused) || !currentPiece) return false;

    SavedGame g;
    g.width = board.getWidth();
    g.height = board.getHeight();
    for (int y = 0; y < g.height; ++y)
        for (int x = 0; x < g.width; ++x) g.cells.push_back(board.getCell(x, y));
    g.pieceType = static_cast<int>(currentPiece->getType());
    g.pieceRotation = currentPiece->getRotation();
    g.pieceX = currentPiece->getX();
    g.pieceY = currentPiece->getY();
    g.nextType = static_cast<int>(nextType);
    g.score = scoreManager.getScore();
    g.lines = scoreManager.getLines();
    return SaveSystem::saveToFile(savePath, g);

}

bool GameEngine::loadGame() {
    SavedGame g;
    if (!SaveSystem::loadFromFile(savePath, g)) return false;
    if (g.width != board.getWidth() || g.height != board.getHeight()) return false;

    Tetromino piece(static_cast<TetrominoType>(g.pieceType), g.pieceX, g.pieceY);
    for (int i = 0; i < g.pieceRotation; ++i) piece.rotate();

    Board check;
    for (int y = 0; y < g.height; ++y)
        for (int x = 0; x < g.width; ++x) check.setCell(x, y, g.cells[y * g.width + x]);
    if (!check.fits(piece)) return false;

    for (int y = 0; y < g.height; ++y)
        for (int x = 0; x < g.width; ++x) board.setCell(x, y, g.cells[y * g.width + x]);

    currentPiece = std::make_unique<Tetromino>(piece);
    nextType = static_cast<TetrominoType>(g.nextType);
    scoreManager.restore(g.score, g.lines);
    fallTime = 0.0f;
    lockTimer = 0.0f;
    newRecord = false;
    state = GameState::Playing;
    return true;
}

void GameEngine::triggerCatEvent() {
    std::unique_ptr<ICatStrategy> strategy;
    
    if (std::rand() % 2 == 0) {
        strategy = std::make_unique<PawPushStrategy>();
        catEffectDuration = 3.0f;
    } else {
        strategy = std::make_unique<SleepOnKeyboardStrategy>();
        catEffectDuration = 5.0f;
    }

    catMessage = strategy->getMessage();
    strategy->apply(*this);
}
