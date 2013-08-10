#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include <time.h>

#include "cocos2d.h"
#include "Background.h"
#include "Ship.h"
#include "Bullet.h"

using namespace cocos2d;

class GameLayer : public CCLayer {
	Background *background;
	Ship *ship;

	CCArray *shipBullets;
	CCArray *enemies;
	CCArray *enemyBullets;

	CCSize screenSize;

	time_t origTime;

public:
    ~GameLayer();
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameLayer);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *event);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *event);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *event);

	void updateShip();
    void updateHealth();
	
	void addEnemies();
	void updateEnemies();

	void update(float dt);
};

#endif
