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
    screenCenter = ccp(screenSize.width/2, screenSize.height/2);

    this->initBackground();
    this->initMessageLabel();
    this->initScoreLabel();
    this->initPaddles();
    this->initBall();
    this->initAudio();
    
    fadeOut = CCFadeOut::create(0.75f);
    fadeOut->retain();
    
    fadeIn = CCFadeIn::create(0.75f);
    fadeIn->retain();
    
    this->setTouchEnabled(true);
    this->scheduleUpdate();

    return true;
}

void GameLayer::initBackground() {
    CCSprite* background = CCSprite::create("space_background.png");
    background->setAnchorPoint(ccp(0.0f, 0.0f));
    background->setPosition(ccp(0.0f, 0.0f));
    
    // scale background to cover the whole window.
    CCRect bgRect = background->getTextureRect();
    background->setScaleX(screenSize.width / bgRect.size.width);
    background->setScaleY(screenSize.height / bgRect.size.height);
    this->addChild(background, 0);
}

void GameLayer::initMessageLabel() {
    messageLabel = CCLabelBMFont::create("CLICK TO PLAY", "visitor.fnt");
    messageLabel->setScale(2.0f);
    messageLabel->setAnchorPoint(ccp(0.5, 0.5));
    messageLabel->setPosition(ccp(screenCenter.x, screenCenter.y));
    this->addChild(messageLabel);
}

void GameLayer::initScoreLabel() {
    scoreLabel = CCLabelBMFont::create("0", "visitor.fnt");
    scoreLabel->setScale((screenSize.width/screenSize.height)*2);
    scoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    scoreLabel->setPosition(ccp(screenCenter.x, screenCenter.y));
    scoreLabel->setOpacity(100);
    scoreLabel->setVisible(false);
    this->addChild(scoreLabel);
}

void GameLayer::initPaddles() {    
    paddleBot = Paddle::createPaddle("paddle.png");
    paddleBot->setScale(screenSize.width/(182.0f*4));
    paddleBot->setHeight(paddleBot->boundingBox().size.height);
    paddleBot->setWidth(paddleBot->boundingBox().size.width);
    paddleBot->setPosition(ccp(screenCenter.x, paddleBot->getHeight() / 2));
    this->addChild(paddleBot);
    
    paddleTop = Paddle::createPaddle("paddle.png");
    paddleTop->setScale(screenSize.width/(182.0f*4));
    paddleTop->setHeight(paddleTop->boundingBox().size.height);
    paddleTop->setWidth(paddleTop->boundingBox().size.width);    paddleTop->setRotation(180.f);
    paddleTop->setPosition(ccp(screenCenter.x, screenSize.height - (paddleTop->getHeight() / 2)));
    this->addChild(paddleTop);
}

void GameLayer::initBall() {
    ball = Ball::createBall("ball_30x30.png");
    ball->setPosition(ccp(screenCenter.x, screenCenter.y - 80));
    
    // set velocity relative to screen size (slower in small devices, faster in big ones)
    float vx = screenSize.width - ball->boundingBox().size.width;
    vx = (vx/ball->boundingBox().size.width) / 4;
    float vy = screenSize.height - ball->boundingBox().size.height;
    vy = (vy/ball->boundingBox().size.height) / 6;
    gameSpeed = ccp(vx, vy);
    ball->setVelocity(gameSpeed);
    
    this->addChild(ball);
}

void GameLayer::initAudio() {
    _sharedEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
    _sharedEngine->preloadEffect("pong.wav");
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
        float posY = paddleTop->getPositionY() - paddleTop->getHeight() / 2 - (ballHeight / 2);
        ball->bounce(posY);
        this->updateScore();
    }
    
    if (this->checkCollision(paddleBot)) {
        float posY = paddleBot->getPositionY() + paddleBot->getHeight() / 2 + (ballHeight / 2);
        ball->bounce(posY);
        this->updateScore();
    }

    // check screen bounds
    float l = ballWidth/2;
    float r = screenSize.width - l;
    float t = screenSize.height;
    float b = 0.0f;
    
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
     ball->setVelocity(gameSpeed);
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