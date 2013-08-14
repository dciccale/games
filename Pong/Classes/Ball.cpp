#include "Ball.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

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

void Ball::bounce(float posY) {
    SimpleAudioEngine::sharedEngine()->playEffect("pong.wav");
    this->setVelocity(ccp(this->getVelocity().x, this->getVelocity().y * -1));
    this->setPositionY(posY);
}