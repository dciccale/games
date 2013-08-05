#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "Spaceship.h"
#include "Koala.h"

using namespace cocos2d;

class GameLayer : public cocos2d::CCLayer {

	Spaceship* spaceShip;
	Koala* koala;
	CCSize screenSize;
  CCSprite *bg;
  CCTMXTiledMap *_tileMap;
  CCTMXLayer *_background;

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
