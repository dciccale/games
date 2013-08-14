#include "GameLayer.h"

USING_NS_CC;

CCScene* GameLayer::scene() {
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

bool GameLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    running = false;

    screenSize = CCDirector::sharedDirector()->getWinSize();

    messageLabel = CCLabelBMFont::create("CLICK TO PLAY", "visitor.fnt");
    messageLabel->setAnchorPoint(ccp(0.5, 0.5));
    messageLabel->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
    this->addChild(messageLabel);

    fadeOut = CCFadeOut::create(0.75f);
    fadeOut->retain();

    fadeIn = CCFadeIn::create(0.75f);
    fadeIn->retain();

    scoreLabel = CCLabelBMFont::create("0", "visitor.fnt");
    scoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    scoreLabel->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
    scoreLabel->setOpacity(120);
    scoreLabel->setVisible(false);
    this->addChild(scoreLabel);

    float pWidth = screenSize.width/5;
    float pHeight = screenSize.height/30;
    
    paddleBot = Paddle::createPaddle(pWidth, pHeight);
    paddleBot->setPosition(ccp(screenSize.width / 2, paddleBot->getHeight() / 2));
    this->addChild(paddleBot);
     
    paddleTop = Paddle::createPaddle(pWidth, pHeight);
    paddleTop->setPosition(ccp(screenSize.width / 2, screenSize.height - (paddleTop->getHeight() / 2)));
    this->addChild(paddleTop);

    ball = Ball::createBall(5.0f);
    ball->setPosition(ccp(screenSize.width / 2, screenSize.height / 2 - 100));
    this->addChild(ball);

    this->setTouchEnabled(true);
    this->schedule(schedule_selector(GameLayer::update));

    return true;
}

bool GameLayer::checkCollision(Paddle *paddle) {
    bool ret = false;

    CCPoint paddlePos = paddle->getPosition();
    float width = paddle->getWidth() / 2;
    float height = paddle->getHeight() / 2;
    float left = paddlePos.x - width;
    float right = paddlePos.x + width;

    CCPoint ballPos = ball->getPosition();
    float rad = ball->getRadius() / 2;


    if (ballPos.x - rad >= left && ballPos.x + rad <= right) {
        if (ballPos.y >= paddlePos.y - height && paddle == paddleTop) {
            ret = true;
        }    

        if (ballPos.y <= paddlePos.y + height && paddle == paddleBot) {
            ret = true;
        }
    }
    return ret;
}

void GameLayer::updateScore() {
    score += 1;
    char s[100] = {0};
    sprintf(s, "%i", score);
    scoreLabel->setString(s, true);

}

void GameLayer::update(float dt) {    
    if (!running) {
        return;
    }

    // move ball
    ball->setPositionX(ball->getPositionX() + ball->getVelocity().x);
    ball->setPositionY(ball->getPositionY() + ball->getVelocity().y);

    // check paddle bounds
    if (this->checkCollision(paddleTop)) {
        _sharedEngine->playEffect("pong.wav");
        ball->setVelocity(ccp(ball->getVelocity().x, ball->getVelocity().y * -1));
        ball->setPositionY(paddleTop->getPositionY() - paddleTop->getHeight() / 2);
        this->updateScore();

    } 
    if (this->checkCollision(paddleBot)) {
        _sharedEngine->playEffect("pong.wav");
        ball->setVelocity(ccp(ball->getVelocity().x, ball->getVelocity().y * -1));
        ball->setPositionY(paddleBot->getPositionY() + paddleBot->getHeight() / 2);
        this->updateScore();
    }

    // check screen bounds
    float l = ball->getRadius() / 2;
    float r = screenSize.width - (ball->getRadius() / 2);
    float t = screenSize.height - (ball->getRadius() / 2);
    float b = ball->getRadius() / 2;

    if (ball->getPositionX() < l) {
        ball->setPositionX(l);
        ball->setVelocity(ccp(ball->getVelocity().x * -1, ball->getVelocity().y));
    } else if (ball->getPositionX() > r) {
        ball->setPositionX(r);
        ball->setVelocity(ccp(ball->getVelocity().x * -1, ball->getVelocity().y));
    }

    if (ball->getPositionY() < b) {
        ball->setPositionY(b);
        ball->setVelocity(ccp(ball->getVelocity().x, ball->getVelocity().y * -1));
        this->stopGame();
    } else if (ball->getPositionY() > t) {
        ball->setPositionY(t);
        ball->setVelocity(ccp(ball->getVelocity().x, ball->getVelocity().y * -1)); 
        this->stopGame();
    }
    
}

void GameLayer::stopGame() {
     running = false;
     
     messageLabel->setString("GAME OVER");
     CCSequence *sequence = CCSequence::createWithTwoActions(
        (CCFiniteTimeAction *) fadeIn->copy()->autorelease(),
        CCCallFuncN::create(this, callfuncN_selector(GameLayer::fadeInDone))
      );
      messageLabel->runAction(sequence);
     
      // reset ball
     ball->setVisible(false);
     ball->setVelocity(ccp(4, 2));
     ball->setPosition(ccp(screenSize.width / 2, (screenSize.height / 2) - 100));

     paddleTop->setPositionX(screenSize.width / 2);
     paddleBot->setPositionX(screenSize.width / 2);
}

void GameLayer::resetGame() {
    ball->setVisible(true);
    score = 0;
    char s[100] = {0};
    sprintf(s, "%i", score);
    scoreLabel->setString(s);
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *event) {
    CCTouch *touch = (CCTouch*) pTouches->anyObject();
    if (touch) {
        if (!running) {
            CCSequence *sequence = CCSequence::createWithTwoActions(
                (CCFiniteTimeAction *) fadeOut->copy()->autorelease(),
                CCCallFuncN::create(this, callfuncN_selector(GameLayer::fadeOutDone)));
            messageLabel->runAction(sequence);
        } else {
            float x = touch->getLocation().x;
            paddleTop->setPositionX(x);
            paddleBot->setPositionX(x);
        }

    }
}

void GameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *event) {
    CCTouch *touch = (CCTouch*) pTouches->anyObject();
    if (touch && running) {
            float x = touch->getLocation().x;
            paddleTop->setPositionX(x);
            paddleBot->setPositionX(x);
        
    }
}

void GameLayer::fadeOutDone(CCNode *pSender) {
    pSender->stopAllActions();
    messageLabel->setOpacity(0);
    running = true;
    scoreLabel->setVisible(true);
    this->resetGame();
}

void GameLayer::fadeInDone(CCNode *pSender) {
    pSender->stopAllActions();
    running = false;
    scoreLabel->setVisible(false);
}

