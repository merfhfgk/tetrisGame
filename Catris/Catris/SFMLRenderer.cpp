#include "SFMLRenderer.h"

SFMLRenderer::SFMLRenderer() {
    unsigned int width = static_cast<unsigned int>(10 * blockSize);
    unsigned int height = static_cast<unsigned int>(20 * blockSize);
    
    window.create(sf::VideoMode({width, height}), "Catris");
    window.setFramerateLimit(60);
}

void SFMLRenderer::run(GameEngine& engine) {
    engine.start();
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Left:  engine.handleInput(Action::Left); break;
                    case sf::Keyboard::Key::Right: engine.handleInput(Action::Right); break;
                    case sf::Keyboard::Key::Down:  engine.handleInput(Action::SoftDrop); break;
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

        engine.update(dt);

        window.clear(sf::Color(31, 30, 29));
        renderBoard(engine);
        renderPiece(engine);
        window.display();
    }
}

void SFMLRenderer::renderBoard(const GameEngine& engine) {
    const Board& board = engine.getBoard();
    sf::RectangleShape cell(sf::Vector2f({blockSize - 1.0f, blockSize - 1.0f}));

    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            int cellValue = board.getCell(x, y);
            cell.setPosition({x * blockSize, y * blockSize});
            
            if (cellValue == 0) {
                cell.setFillColor(sf::Color(45, 45, 45));
            } else {
                cell.setFillColor(getColor(cellValue));
            }
            window.draw(cell);
        }
    }
}

void SFMLRenderer::renderPiece(const GameEngine& engine) {
    const Tetromino* piece = engine.getCurrentPiece();
    if (!piece) return;

    int typeValue = static_cast<int>(piece->getType()) + 1;
    sf::RectangleShape block(sf::Vector2f({blockSize - 1.0f, blockSize - 1.0f}));
    block.setFillColor(getColor(typeValue));

    for (const auto& p : piece->getBlocks()) {
        block.setPosition({static_cast<float>(p.x) * blockSize, static_cast<float>(p.y) * blockSize});
        window.draw(block);
    }
}

sf::Color SFMLRenderer::getColor(int value) const {
    switch (value) {
        case 1: return sf::Color::Cyan;
        case 2: return sf::Color::Yellow;
        case 3: return sf::Color::Magenta;
        case 4: return sf::Color::Green;
        case 5: return sf::Color::Red;
        case 6: return sf::Color::Blue;
        case 7: return sf::Color(235, 201, 120);
        default: return sf::Color::White;
    }
}
