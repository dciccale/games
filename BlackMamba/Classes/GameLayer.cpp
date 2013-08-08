#include "GameLayer.h"

#include <cstdlib>

USING_NS_CC;

CCScene* GameLayer::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // gradient layer
    CCLayerGradient *layerGrad = new CCLayerGradient();
    layerGrad->initWithColor(ccc4(192,239,254,255), ccc4(255,255,255,255));
    scene->addChild(layerGrad);
    
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

	const char *texture = "Spaceship.png";
    
	spaceShip = Spaceship::create(texture);
	spaceShip->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));


    
	// later we'll use this...
	// int randX = 1 + rand() % 4;
	// int randY = 1 + rand() % 4;
	// spaceShip->setVelocity(ccp(randX, randY));
    
	this->addChild(spaceShip);
    
    // background
    _tileMap = new CCTMXTiledMap();
    _tileMap->initWithTMXFile("background.tmx");
    _background = _tileMap->layerNamed("Background");
    this->addChild(_tileMap);
    
    // koala    
	koala = Koala::create("kuwalio_stand.png");
	koala->setPosition(ccp(screenSize.width-40, 30));
  this->addChild(koala);

	this->setTouchEnabled(true);
    
	this->schedule(schedule_selector(GameLayer::update));      
    
    return true;
}

void GameLayer::update(float dt) {

}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
	CCSetIterator it;
	CCTouch *touch;
	CCPoint tap;
	float scale;
	CCActionManager *mgr = CCDirector::sharedDirector()->getActionManager();

	for (it = pTouches->begin(); it != pTouches->end(); it++) {
		touch = (CCTouch*) (*it);

		if (touch) {
			tap = touch->getLocation();
			
			/*if (spaceShip->boundingBox().containsPoint(tap)) {

				scale = spaceShip->getScale();
				CCScaleTo *scaleTo;

				if (scale > 1) {
					scaleTo = CCScaleTo::create(0.2f, 1, 1);
					mgr->addAction(scaleTo, spaceShip, true);
				} else {
					scaleTo = CCScaleTo::create(0.2f, 1.3, 1.3);
					mgr->addAction(scaleTo, spaceShip, true);
				}
			}*/

			CCMoveTo *moveTo = CCMoveTo::create(0.5f, tap);
			mgr->addAction(moveTo, spaceShip, true);

			mgr->resumeTarget(spaceShip);

		}
	}
}


void GameLayer::ccTouchesEnd(CCSet *pTouches, CCEvent *pEvent) { 

}
