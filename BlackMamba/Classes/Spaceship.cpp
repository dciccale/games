#include "Spaceship.h"


Spaceship::Spaceship(void) { }

Spaceship* Spaceship::create(const char* pszFilename) {
    
    Spaceship* sprite  = new Spaceship();
    if (sprite && sprite->initWithFile(pszFilename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    
    return NULL;
}

