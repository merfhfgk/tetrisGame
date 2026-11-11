//
//  SFMLRenderer.h
//  Catris
//
#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include "IRenderer.h"
#include "GameEngine.h"

class SFMLRenderer : public IRenderer {
private:
    sf::RenderWindow window;
    const float blockSize = 30.0f;

    void renderBoard(const GameEngine& engine);
    void renderPiece(const GameEngine& engine);
    sf::Color getColor(int typeValue) const;

public:
    SFMLRenderer();
    void run(GameEngine& engine) override;
};

#endif

