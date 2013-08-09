#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ScoreLayer.h"

USING_NS_CC;

class GameLayer : public CCLayerColor
{
    CocosDenshion::SimpleAudioEngine *_sharedEngine;
    ScoreLayer *_scoreLayer;
    
public:
    virtual bool init();  

    static CCScene* scene();
    
    void playBgMusic();
    void initScoreLayer();
    
    void menuCloseCallback(CCObject* pSender);
    
    CREATE_FUNC(GameLayer);
};

#endif
