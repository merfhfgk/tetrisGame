#include "SFMLRenderer.h"
#include <iostream>

SFMLRenderer::SFMLRenderer() {
    unsigned int fieldWidth = cols * static_cast<unsigned int>(blockSize);
    unsigned int totalWidth = fieldWidth + static_cast<unsigned int>(sidebarWidth);
    unsigned int height = rows * static_cast<unsigned int>(blockSize);
    
    window.create(sf::VideoMode({totalWidth, height}), "Catris");
    window.setFramerateLimit(60);
    
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    if (!font.openFromFile("/Users/mariia/Desktop/tetrisGame/Catris/Catris/font.ttf")) {
        std::cerr << "Failed to load font.ttf! Please ensure it is in the correct directory." << std::endl;
    }
}

void SFMLRenderer::run(GameEngine& engine) {
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        handleEvents(engine);
        
        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();

            handleEvents(engine);
            handleRealTimeInput(engine, dt);

            engine.update(dt);
            
            render(engine);
        }

        engine.update(dt);

        render(engine);
    }
}

void SFMLRenderer::handleEvents(GameEngine& engine) {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (event->is<sf::Event::FocusLost>()) {
            if (engine.getState() == GameState::Playing) {
                engine.handleInput(Action::Pause);
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            
            if (engine.getState() == GameState::Menu || engine.getState() == GameState::GameOver) {
                if (keyPressed->code == sf::Keyboard::Key::Enter || keyPressed->code == sf::Keyboard::Key::Space) {
                    engine.start();
                } else if (keyPressed->code == sf::Keyboard::Key::Q) {
                    window.close();
                } else if (keyPressed->code == sf::Keyboard::Key::L) {
                    engine.handleInput(Action::Load);
                }
                continue;
            }

            switch (keyPressed->code) {
                case sf::Keyboard::Key::Left:
                case sf::Keyboard::Key::A:     engine.handleInput(Action::Left); break;
                case sf::Keyboard::Key::Right:
                case sf::Keyboard::Key::D:     engine.handleInput(Action::Right); break;
                case sf::Keyboard::Key::Down:
                case sf::Keyboard::Key::S:     engine.handleInput(Action::SoftDrop); break;
                case sf::Keyboard::Key::Up:
                case sf::Keyboard::Key::W:     engine.handleInput(Action::Rotate); break;
                case sf::Keyboard::Key::Space:
                case sf::Keyboard::Key::X:     engine.handleInput(Action::HardDrop); break;
                case sf::Keyboard::Key::P:     engine.handleInput(Action::Pause); break;
                case sf::Keyboard::Key::K:     engine.handleInput(Action::Save); break;
                case sf::Keyboard::Key::L:     engine.handleInput(Action::Load); break;
                case sf::Keyboard::Key::Q:     engine.handleInput(Action::Quit); window.close(); break;
                default: break;
            }
        }
    }
}

void SFMLRenderer::handleRealTimeInput(GameEngine& engine, float dt) {
    if (engine.getState() != GameState::Playing) {
        moveTimer = 0.0f;
        return;
    }

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);

    if (left || right || down) {
        moveTimer += dt;
        
        float currentThreshold = (moveTimer > initialDelay) ? repeatDelay : initialDelay;
        
        if (moveTimer > currentThreshold) {
            if (left) engine.handleInput(Action::Left);
            else if (right) engine.handleInput(Action::Right);
            else if (down) engine.handleInput(Action::SoftDrop);
            
            moveTimer = (moveTimer > initialDelay) ? initialDelay : 0.0f;
        }
    } else {
        moveTimer = 0.0f; 
    }
}

void SFMLRenderer::render(const GameEngine& engine) {
    window.clear(sf::Color(31, 30, 29));

    renderBoard(engine);
    renderGhostPiece(engine);
    renderPiece(engine);
    renderSidebar(engine);
    renderOverlay(engine);

    window.display();
}

void SFMLRenderer::renderBoard(const GameEngine& engine) {
    const Board& board = engine.getBoard();
    sf::RectangleShape cell(sf::Vector2f({blockSize - 1.0f, blockSize - 1.0f}));

    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            int cellValue = board.getCell(x, y);
            cell.setPosition({x * blockSize, y * blockSize});
            
            if (cellValue == 0) {
                cell.setFillColor(sf::Color(42, 40, 38));
            } else {
                cell.setFillColor(getColor(cellValue));
            }
            window.draw(cell);
        }
    }

    sf::RectangleShape line(sf::Vector2f({2.0f, static_cast<float>(rows) * blockSize}));
    line.setPosition({static_cast<float>(cols) * blockSize, 0.0f});
    line.setFillColor(sf::Color(70, 68, 64));
    window.draw(line);
}

void SFMLRenderer::renderPiece(const GameEngine& engine) {
    const Tetromino* piece = engine.getCurrentPiece();
    if (!piece || engine.getState() != GameState::Playing) return;

    int typeValue = static_cast<int>(piece->getType()) + 1;
    sf::RectangleShape block(sf::Vector2f({blockSize - 1.0f, blockSize - 1.0f}));
    block.setFillColor(getColor(typeValue));

    for (const auto& p : piece->getBlocks()) {
        block.setPosition({static_cast<float>(p.x) * blockSize, static_cast<float>(p.y) * blockSize});
        window.draw(block);
    }
}
    
void SFMLRenderer::renderGhostPiece(const GameEngine& engine) {
    const Tetromino* current = engine.getCurrentPiece();
    if (!current || engine.getState() != GameState::Playing) return;

    Tetromino ghost = *current;
    const Board& board = engine.getBoard();

    Tetromino candidate = ghost;
    candidate.move(0, 1);
    while (board.fits(candidate)) {
        ghost = candidate;
        candidate.move(0, 1);
    }

    int typeValue = static_cast<int>(ghost.getType()) + 1;
    sf::Color ghostColor = getColor(typeValue);
    ghostColor.a = 150;
    
    sf::RectangleShape block(sf::Vector2f({blockSize - 1.0f, blockSize - 1.0f}));
    block.setFillColor(sf::Color(255, 255, 255, 10)); // Ледь помітна біла заливка
    block.setOutlineThickness(1.0f);
    block.setOutlineColor(ghostColor);
    
    for (const auto& p : ghost.getBlocks()) {
        block.setPosition({static_cast<float>(p.x) * blockSize, static_cast<float>(p.y) * blockSize});
        window.draw(block);
    }
}

void SFMLRenderer::renderNextPiece(TetrominoType nextType, float startX, float startY) {
    Tetromino tempPiece(nextType, 0, 0);
    sf::RectangleShape block(sf::Vector2f({blockSize - 1.0f, blockSize - 1.0f}));
    block.setFillColor(getColor(static_cast<int>(nextType) + 1));

    for (const auto& p : tempPiece.getBlocks()) {
        block.setPosition({startX + p.x * blockSize, startY + p.y * blockSize});
        window.draw(block);
    }
}

sf::Text SFMLRenderer::createText(const std::string& str, unsigned int size, float x, float y, sf::Color color) {
    sf::Text text(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition({x, y});
    return text;
}

void SFMLRenderer::renderSidebar(const GameEngine& engine) {
    float originX = cols * blockSize + 20.0f;
    sf::Color cream(240, 235, 225);
    sf::Color gold(235, 201, 120);

    window.draw(createText("NEXT", 16, originX + 65.0f, 10.0f, cream));
    sf::RectangleShape nextBox(sf::Vector2f({180.0f, 90.0f}));
    nextBox.setPosition({originX, 35.0f});
    nextBox.setFillColor(sf::Color(45, 43, 40));
    nextBox.setOutlineThickness(1.0f);
    nextBox.setOutlineColor(gold);
    window.draw(nextBox);
    renderNextPiece(engine.getNextType(), originX + 75.0f, 60.0f);

    window.draw(createText("CAT INVASION", 14, originX + 35.0f, 145.0f, sf::Color(215, 120, 140)));
    sf::RectangleShape catBox(sf::Vector2f({180.0f, 60.0f}));
    catBox.setPosition({originX, 165.0f});
    catBox.setFillColor(sf::Color(55, 40, 45));
    catBox.setOutlineThickness(1.0f);
    catBox.setOutlineColor(sf::Color(215, 120, 140));
    window.draw(catBox);

    const ScoreManager& sm = engine.getScoreManager();
    window.draw(createText("SCORE", 14, originX, 250.0f, gold));
    window.draw(createText(std::to_string(sm.getScore()), 22, originX, 270.0f, cream));

    window.draw(createText("BEST", 14, originX, 310.0f, gold));
    window.draw(createText(std::to_string(sm.getHighScore()), 22, originX, 330.0f, cream));

    window.draw(createText("LEVEL", 14, originX, 370.0f, gold));
    window.draw(createText(std::to_string(sm.getLevel()), 22, originX, 390.0f, cream));

    window.draw(createText("LINES", 14, originX, 430.0f, gold));
    window.draw(createText(std::to_string(sm.getLines()), 22, originX, 450.0f, cream));
    
    window.draw(createText("Controls:\nWASD - Move/Drop\nSpace - Hard Drop\nP - Pause   K - Save\nL - Load    Q - Quit", 12, originX, 510.0f, sf::Color(150, 150, 150)));
}

void SFMLRenderer::renderOverlay(const GameEngine& engine) {
    GameState state = engine.getState();
    if (state == GameState::Playing) return;

    sf::RectangleShape overlay(sf::Vector2f({cols * blockSize, rows * blockSize}));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    sf::Color cream(240, 235, 225);
    sf::Color gold(235, 201, 120);

    if (state == GameState::Menu) {
        window.draw(createText("CATRIS", 40, 65.0f, 200.0f, gold));
        window.draw(createText("Press SPACE to Start", 18, 45.0f, 260.0f, cream));
        window.draw(createText("Press L to Load Save", 16, 60.0f, 300.0f, sf::Color(150, 150, 150)));
    }
    else if (state == GameState::Paused) {
        window.draw(createText("PAUSED", 36, 75.0f, 250.0f, gold));
        window.draw(createText("Press P to Resume", 16, 65.0f, 300.0f, cream));
    }
    else if (state == GameState::GameOver) {
        window.draw(createText("GAME OVER", 36, 45.0f, 200.0f, sf::Color::Red));
        
        if (engine.getNewRecord()) {
            window.draw(createText("NEW RECORD!", 24, 60.0f, 260.0f, gold));
        }
        
        window.draw(createText("Press SPACE to Restart", 16, 50.0f, 320.0f, cream));
    }
}

sf::Color SFMLRenderer::getColor(int value) const {
    switch (value) {
        case 1: return sf::Color(100, 200, 230);
        case 2: return sf::Color(240, 210, 90);
        case 3: return sf::Color(180, 100, 210);
        case 4: return sf::Color(100, 210, 120);
        case 5: return sf::Color(230, 90, 90);
        case 6: return sf::Color(90, 130, 230);
        case 7: return sf::Color(235, 201, 120);
        default: return sf::Color::White;
    }
}
