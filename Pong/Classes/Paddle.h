#ifndef __PADDLE_H__
#define __PADDLE_H__

#include "cocos2d.h"
#include "Ball.h"

USING_NS_CC;

class Paddle : public CCSprite {
public:
    CC_SYNTHESIZE(float, width, Width);
    CC_SYNTHESIZE(float, height, Height);

    Paddle();
    static Paddle* createPaddle(const char *pszFileName);
};

#endif