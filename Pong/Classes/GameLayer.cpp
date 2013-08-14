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
    
    // Load the background image sprite and set the position to bottom left side of the screen.
    CCSprite* background = CCSprite::create("space_background.png");
    background->setAnchorPoint(ccp(0.0f, 0.0f));
    background->setPosition(ccp(0.0f, 0.0f));
    
    // Scale background to cover the whole window.
    CCRect bgRect = background->getTextureRect();
    background->setScaleX(screenSize.width / bgRect.size.width);
    background->setScaleY(screenSize.height / bgRect.size.height);
    
    // Add the background as a child to JumpingGame layer.
    this->addChild(background, 0);

    messageLabel = CCLabelBMFont::create("CLICK TO PLAY", "visitor.fnt");
    messageLabel->setAnchorPoint(ccp(0.5, 0.5));
    messageLabel->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
    this->addChild(messageLabel);

    fadeOut = CCFadeOut::create(0.75f);
    fadeOut->retain();

    fadeIn = CCFadeIn::create(0.75f);
    fadeIn->retain();

    scoreLabel = CCLabelBMFont::create("0", "visitor.fnt");
    scoreLabel->setScale(3.5f);
    scoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    scoreLabel->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
    scoreLabel->setOpacity(100);
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

    ball = Ball::createBall("ball_30x30.png");
    ball->setPosition(ccp(screenSize.width / 2, screenSize.height / 2 - 100));
    this->addChild(ball);

    this->setTouchEnabled(true);
    this->schedule(schedule_selector(GameLayer::update));
    
    _sharedEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();

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
    float ballTop = ball->boundingBox().size.height/2;


    if (ballPos.x >= left && ballPos.x <= right) {
        if (ballPos.y + ballTop >= paddlePos.y - height && paddle == paddleTop) {
            ret = true;
        }

        if (ballPos.y - ballTop <= paddlePos.y + height && paddle == paddleBot) {
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


    // CCLog("test: %i ", score % 4 == 0);

}

void GameLayer::update(float dt) {    
    if (!running) {
        return;
    }


    float x = paddleTop->getPositionX();
    if (x < 50) {
        x = 50;
    } else if (x > screenSize.width - 50) {
        x = screenSize.width - 50;
    }
    paddleTop->setPositionX(x);
    paddleBot->setPositionX(paddleTop->getPositionX());

    // move ball
    ball->setPositionX(ball->getPositionX() + ball->getVelocity().x);
    ball->setPositionY(ball->getPositionY() + ball->getVelocity().y);
    float ballWidth = ball->boundingBox().size.width;
    float ballHeight = ball->boundingBox().size.height;


    // check paddle bounds
    if (this->checkCollision(paddleTop)) {
        _sharedEngine->playEffect("pong.wav");
        ball->setVelocity(ccp(ball->getVelocity().x, ball->getVelocity().y * -1));
        ball->setPositionY(paddleTop->getPositionY() - paddleTop->getHeight() / 2 - (ballHeight / 2));
        this->updateScore();
    } 
    if (this->checkCollision(paddleBot)) {
        _sharedEngine->playEffect("pong.wav");
        ball->setVelocity(ccp(ball->getVelocity().x, ball->getVelocity().y * -1));
        ball->setPositionY(paddleBot->getPositionY() + paddleBot->getHeight() / 2 + (ballHeight / 2));
        this->updateScore();
    }

    // check screen bounds
    float l = ballWidth/2;
    float r = screenSize.width - ballWidth/2;
    float t = screenSize.height;
    float b = 0.0f;

    /*sprintf(text, "%f\n", ball->boundingBox().size.width);
    CCLOG(text);*/
    
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