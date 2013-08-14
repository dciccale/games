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

/*
Paddle* Paddle::createPaddle(float width, float height) {    
    Paddle *sprite = new Paddle();
    if (sprite && sprite->init()) {
        sprite->setWidth(width);
        sprite->setHeight(height);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void Paddle::draw() {
    float w = this->getWidth() / 2;
    float h = this->getHeight() / 2;
    ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    ccDrawSolidRect(ccp(-w, -h), ccp(w, h), ccc4f(1.0f, 1.0f, 1.0f, 1.0f));
}
*/