#include "GameLayer.h"

#include <cstdlib>

USING_NS_CC;

CCScene* GameLayer::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

bool GameLayer::init() {

    if (!CCLayer::init()) {
        return false;
    }


    screenSize = CCDirector::sharedDirector()->getWinSize();

	const char* texture = "Spaceship.png";
    
	spaceShip = Spaceship::create(texture);
	spaceShip->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
    
	// later we'll use this...
	// int randX = 1 + rand() % 4;
	// int randY = 1 + rand() % 4;
	// spaceShip->setVelocity(ccp(randX, randY));
    
	this->addChild(spaceShip);


	this->setTouchEnabled(true);
	this->schedule(schedule_selector(GameLayer::update));      
    
    return true;
}

void GameLayer::update(float dt) {

}

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent) {
	CCSetIterator it;
	CCTouch* touch;
	CCPoint tap;
	float scale;
	float rotation;
	CCActionManager* mgr = CCDirector::sharedDirector()->getActionManager();
	


	for (it = pTouches->begin(); it != pTouches->end(); it++) {
		touch = (CCTouch*) (*it);
		if (touch) {
			tap = touch->getLocation();
			
			int randX = 1 + rand() % 360;
			int randY = 1 + rand() % 3;

			if (spaceShip->boundingBox().containsPoint(tap)) {
				scale = spaceShip->getScale();
				rotation = spaceShip->getRotationX();
				CCScaleTo* scaleTo;

				if (scale > 1) {
					scaleTo = CCScaleTo::create(0.2f, 1, 1);
					mgr->addAction(scaleTo, spaceShip, true);
					// spaceShip->runAction(scaleTo);
				} else {
					scaleTo = CCScaleTo::create(0.2f, 3, 3);
					mgr->addAction(scaleTo, spaceShip, true);
					// spaceShip->runAction(scaleTo);
				}
				
				mgr->resumeTarget(spaceShip);
			}
		}
	}
}

