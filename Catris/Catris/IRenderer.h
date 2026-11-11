//
//  IRenderer.h
//  Catris
//

#ifndef IRENDERER_H
#define IRENDERER_H

class GameEngine;

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void run(GameEngine& engine) = 0;
};

#endif
