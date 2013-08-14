#include "Ball.h"

Ball::Ball() {
    velocity = ccp(4, 2);
}

Ball* Ball::createBall(const char *pszFileName) {
    Ball* sprite = new Ball();
    if (sprite && sprite->initWithFile(pszFileName)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}