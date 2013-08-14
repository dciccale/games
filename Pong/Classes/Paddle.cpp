#include "Paddle.h"

Paddle::Paddle() {}

Paddle* Paddle::createPaddle(const char *pszFileName) {
    Paddle *sprite = new Paddle();
    if (sprite && sprite->initWithFile(pszFileName)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}
