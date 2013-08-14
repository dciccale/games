#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
USING_NS_CC;

class Ball : public CCSprite {

public:
    CC_SYNTHESIZE(CCPoint, velocity, Velocity);
    Ball();
    static Ball* createBall(const char *pszFileName);
};

#endif