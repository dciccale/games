#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
USING_NS_CC;

class Ball : public CCSprite {

public:
    CC_SYNTHESIZE(float, radius, Radius);
    CC_SYNTHESIZE(CCPoint, velocity, Velocity);
    Ball();
    static Ball* createBall(float r);
    virtual void draw();
};

#endif