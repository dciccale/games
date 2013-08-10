#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

enum {
    kSpriteBomb,
    kSpriteShockwave,
    kSpriteMeteor,
    kSpriteHealth,
    kSpriteHalo,
    kSpriteSparkle
};

enum {
    kBackground,
    kMiddleground,
    kForeground
};

class GameLayer : public CCLayer {

    CCArray *meteorPool;
    int meteorPoolIndex;

    CCArray *healthPool;
    int healthPoolIndex;

    CCArray *fallingObjects;
    CCArray *clouds;
    CCSpriteBatchNode *gameBatchNode;
    CCSprite *bomb;
    CCSprite *shockWave;
    CCSprite *introMessage;
    CCSprite *gameOverMessage;
    CCLabelBMFont *energyDisplay;
    CCLabelBMFont *scoreDisplay;

    CCAction *growBomb;
    CCAction *rotateSprite;
    CCAction *shockwaveSequence;
    CCAction *swingHealth;
    CCAction *groundHit;
    CCAction *explosion;

    CCSize screenSize;

    float meteorInterval;
    float meteorSize;
    float meteorTimer;
    float meteorSpeed;

    float healthInterval;
    float healthTimer;
    float healthSpeed;
    
    float difficultyInterval;
    float difficultyTimer;

    int energy;
    int score;
    int shockwaveHits;
    bool running;

    void resetMeteor();
    void resetHealth();
    void resetGame();
    void stopGame();
    void increaseDifficulty();
    void createGameScreen();
    void createPools();
    void createActions();

public:
    ~GameLayer();

    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameLayer);

    void fallingObjectDone(CCNode *pSender);
    void animationDone(CCNode *pSender);
    void shockwaveDone();

    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *event);
    void update(float dt);
};

#endif