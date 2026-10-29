//
//  main.cpp
//  
//
//  Created by Maria Goncharuk on 23.10.2026.
//

#include "GameEngine.h"
#include "ConsoleRender.h"

int main() {
    GameEngine engine;
    ConsoleRenderer renderer;
    renderer.run(engine);
    return 0;
}
