#include "AppDelegate.h"
#include "cocos2d.h"
//#include "SimpleAudioEngine.h"
#include "GameLayer.h"

USING_NS_CC;
//using namespace CocosDenshion;

AppDelegate::AppDelegate()  { }
AppDelegate::~AppDelegate() { }

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // load sound assets    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("background.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("bombFail.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("bombRelease.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("boom.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("health.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("fire_truck.wav");

    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.4f);

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
