#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "Paddle.h"
#include "Ball.h"
#include <string>

USING_NS_CC;

#define PADDLE_W 100
#define PADDLE_H 5

class GameLayer : public CCLayer {

    CCSize screenSize;
    CCLabelBMFont *messageLabel;
    CCLabelBMFont *scoreLabel;

    bool running;
    int score;

    CCFadeOut *fadeOut;
    CCFadeIn *fadeIn;
    Paddle *paddleTop;
    Paddle *paddleBot;
    Ball *ball;

public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameLayer);


    void update(float dt);
    void resetGame();
    void stopGame();
    bool checkCollision(Paddle *paddle);
    void fadeInDone(CCNode *pSender);
    void fadeOutDone(CCNode *pSender);
    void updateScore();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *event);
};

#endif
