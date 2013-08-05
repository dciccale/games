#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "Spaceship.h"

class GameLayer : public cocos2d::CCLayer {
	Spaceship *spaceShip;
	CCSize screenSize;
	CCSprite *background;

public:
    virtual bool init();  
	static cocos2d::CCScene *scene();
    CREATE_FUNC(GameLayer);

	// event listeners
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *event);
	virtual void ccTouchesEnd(CCSet *pTouches, CCEvent *event);

	

	// custom methods
	void update(float dt);
};
#endif 
