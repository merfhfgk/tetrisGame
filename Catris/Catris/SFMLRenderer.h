//
//  SFMLRenderer.h
//  Catris
//
#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include "IRenderer.h"
#include "GameEngine.h"
#include <string>

class SFMLRenderer : public IRenderer {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    const float blockSize = 30.0f;
    const unsigned int cols = 10;
    const unsigned int rows = 20;
    const float sidebarWidth = 220.0f;

    void handleEvents(GameEngine& engine);
    void render(const GameEngine& engine);
    
    void renderBoard(const GameEngine& engine);
    void renderPiece(const GameEngine& engine);
    void renderSidebar(const GameEngine& engine);
    void renderNextPiece(TetrominoType nextType, float startX, float startY);
    void renderOverlay(const GameEngine& engine);
    
    sf::Text createText(const std::string& str, unsigned int size, float x, float y, sf::Color color);
    
    sf::Color getColor(int typeValue) const;

    float moveTimer = 0.0f;
    const float initialDelay = 0.17f;
    const float repeatDelay = 0.05f;
    
    void handleRealTimeInput(GameEngine& engine, float dt);

    void renderGhostPiece(const GameEngine& engine);
    
public:
    SFMLRenderer();
    void run(GameEngine& engine) override;
};

#endif
