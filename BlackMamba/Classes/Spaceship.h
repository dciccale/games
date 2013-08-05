#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include "cocos2d.h"

using namespace cocos2d;


class Spaceship : public CCSprite {
public:
	CC_SYNTHESIZE(CCPoint, _acceleration, Acceleration);
	CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
	CC_SYNTHESIZE(CCPoint, _velocity, Velocity);	

	Spaceship(void);
	static Spaceship* create(const char* pszFileName);


};
#endif
