#include "Koala.h"


Koala::Koala(void) { }

Koala* Koala::create(const char* pszFilename) {
    
    Koala* sprite  = new Koala();
    if (sprite && sprite->initWithFile(pszFilename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    
    return NULL;
}