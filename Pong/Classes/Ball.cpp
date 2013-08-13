#include "Ball.h"

Ball::Ball() {
    radius = 10.0f;
    velocity = ccp(4, 2);
}

Ball* Ball::createBall(float r) {
    Ball* sprite = new Ball();
    if (sprite && sprite->init()) {
        sprite->setRadius(r);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void Ball::draw() {
    float r = this->getRadius() / 2;
    ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
    ccDrawSolidRect(ccp(-r, -r), ccp(r, r), ccc4f(1.0f, 1.0f, 1.0f, 1.0f));
}
