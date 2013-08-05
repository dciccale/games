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

	this->schedule(schedule_selector(GameLayer::update));      
    
    return true;
}

void GameLayer::update(float dt) {

}

