//
//  main.cpp
//  
//
//  Created by Maria Goncharuk on 23.10.2026.
//

#include <iostream>
#include "GameEngine.h"
#include "SFMLRenderer.h"
// #include "ConsoleRender.h"

int main() {
    GameEngine engine;
    
    SFMLRenderer sfmlRenderer;
    // ConsoleRenderer consoleRenderer;
    
    IRenderer* renderer = &sfmlRenderer;
    
    renderer->run(engine);
    const ScoreManager& sm = engine.getScoreManager();
    std::cout << "Score: " << sm.getScore() << "   Record: " << sm.getHighScore();
    if (engine.getNewRecord()) std::cout << "   *** NEW RECORD ***";
    std::cout << std::endl;
    
    return 0;
}
