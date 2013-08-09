#include "GameLayer.h"

USING_NS_CC;

#define COCOS2D_DEBUG 1

CCScene* GameLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    GameLayer *layer = GameLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameLayer::init()
{

    if ( !CCLayerColor::initWithColor(ccc4(0,0,0,0)))
    {
        return false;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    CCScheduler *_scheduler = CCDirector::sharedDirector()->getScheduler();
    CocosDenshion::SimpleAudioEngine *_sharedEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    
    /// arcade button
    CCMenuItemImage *closeItem = CCMenuItemImage::create(
                                        "button.png",
                                        "button_pressed.png",
                                        this,
                                        menu_selector(GameLayer::menuCloseCallback));   
	closeItem->setPosition(ccp(screenSize.width/2, screenSize.height/2 - 20));

    // menu
    CCMenu* menu = CCMenu::create(closeItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);

    // main text label
    CCLabelTTF *label = CCLabelTTF::create("hey wanna hear the most annoying sound in the world?", "arial", 34, CCSizeMake(screenSize.width-40, screenSize.height/2), kCCTextAlignmentCenter, kCCVerticalTextAlignmentTop);
    label->setPosition(ccp(screenSize.width / 2, screenSize.height - (label->getDimensions().height/2) - 20));
    this->addChild(label, 1);
    
    this->initScoreLayer();

    this->playBgMusic();
    
    return true;
}

void GameLayer::playBgMusic()
{
    _sharedEngine->playBackgroundMusic("dd_annoy_loop.wav", true);
    _sharedEngine->pauseBackgroundMusic();
}

void GameLayer::initScoreLayer()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
    _scoreLayer = new ScoreLayer();
    CCLabelTTF *scoreLabel = _scoreLayer->_label;
    scoreLabel->setAnchorPoint(ccp(0.0f, 0.0f));
    scoreLabel->setPosition(ccp(screenSize.width - scoreLabel->getTextureRect().size.width, 0.0f));
    this->addChild(scoreLabel, 1);
    
    // score update schedule
    CCScheduler *_scheduler = CCDirector::sharedDirector()->getScheduler();
    _scheduler->scheduleSelector(schedule_selector(ScoreLayer::update), _scoreLayer, _scoreLayer->getInterval(), kCCRepeatForever, 0.0f, true);
}

void GameLayer::menuCloseCallback(CCObject* pSender)
{
    CCScheduler *_scheduler = CCDirector::sharedDirector()->getScheduler();
    
    //printf("accion: %s",(_sharedEngine->isBackgroundMusicPlaying()) ? "pause" : "resume");
    // isBackgroundMusicPlaying doesnt seem to work for mac apps
    // (always returns true so it doesn't plays the music)
    
    if (_sharedEngine->isBackgroundMusicPlaying()) {
        _sharedEngine->pauseBackgroundMusic();
        _scheduler->pauseTarget(_scoreLayer);
    }
    else {
        _sharedEngine->resumeBackgroundMusic();
        _scheduler->resumeTarget(_scoreLayer);
    }
}