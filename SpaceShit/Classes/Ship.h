#ifndef __SHIP_H__
#define __SHIP_H__

#include "cocos2d.h"

using namespace cocos2d;

// TODO: next time instead of extending CCSprite
//       use CCSprite::setUserData 
class Ship : public CCSprite {
    CCArray *bullets;
public:
	CC_SYNTHESIZE(CCPoint, angle, Angle);
	CC_SYNTHESIZE(int, health, Health);
	CC_SYNTHESIZE(CCPoint, range, Range);
	CC_SYNTHESIZE(CCPoint, speed, Speed);
	CC_SYNTHESIZE(CCTouch*, touch, Touch);
	CC_SYNTHESIZE(CCPoint, velocity, Velocity);
  
	Ship(void);
	static Ship* createShip(const char* pszFilename);

    // TODO
    // void update();
    // void shoot(); ?
};

#endif;
