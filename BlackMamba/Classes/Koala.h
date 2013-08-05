#ifndef __KOALA_H__
#define __KOALA_H__

#include "cocos2d.h"

using namespace cocos2d;


class Koala : public CCSprite {
public:
	CC_SYNTHESIZE(CCPoint, _acceleration, Acceleration);
	CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
	CC_SYNTHESIZE(CCPoint, _velocity, Velocity);
    
	Koala(void);
	static Koala* create(const char* pszFileName);
    
    
};
#endif