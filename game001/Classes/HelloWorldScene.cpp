#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    
    HelloWorld *layer = HelloWorld::create();
    
    scene->addChild(layer);
    
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,0)) )
    {
        return false;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

    /// Play sound button
    CCMenuItemImage *closeItem = CCMenuItemImage::create(
                                        "button.png",
                                        "button_pressed.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
	closeItem->setPosition(ccp(screenSize.width/2, screenSize.height/2 - 20));

    // create menu, it's an autorelease object
    CCMenu* menu = CCMenu::create(closeItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);
    

    CCLabelTTF *label = CCLabelTTF::create("hey wanna hear the most annoying sound in the world?", "Thonburi", 34, CCSizeMake(screenSize.width-40, screenSize.height/2), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    label->setPosition(ccp(screenSize.width / 2, screenSize.height - (label->getDimensions().height/2) - 20));
    this->addChild(label, 1);

    // play loop sound
    CocosDenshion::SimpleAudioEngine* _sharedEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    //_sharedEngine->preloadBackgroundMusic("dd_annoy_loop.wav");
    _sharedEngine->playBackgroundMusic("dd_annoy_loop.wav", true);
    _sharedEngine->pauseBackgroundMusic();
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    if (_sharedEngine->isBackgroundMusicPlaying()) {
        _sharedEngine->pauseBackgroundMusic();
        
    }
    else {
        _sharedEngine->resumeBackgroundMusic();
    }
}
