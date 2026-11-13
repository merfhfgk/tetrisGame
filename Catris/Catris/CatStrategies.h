//
//  CatStrategies.h
//  Catris
//

#ifndef CAT_STRATEGIES_H
#define CAT_STRATEGIES_H

#include <string>

class GameEngine;

class ICatStrategy {
public:
    virtual ~ICatStrategy() = default;
    
    virtual void apply(GameEngine& engine) = 0;
    
    virtual std::string getMessage() const = 0;
};

#endif

