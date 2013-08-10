#include "GameLayer.h"

GameLayer::~GameLayer() {
    // release retained stuff
    CC_SAFE_RELEASE(growBomb);
    CC_SAFE_RELEASE(rotateSprite);
    CC_SAFE_RELEASE(shockwaveSequence);
    CC_SAFE_RELEASE(swingHealth);
    CC_SAFE_RELEASE(groundHit);
    CC_SAFE_RELEASE(explosion);
    CC_SAFE_RELEASE(clouds);
    CC_SAFE_RELEASE(meteorPool);
    CC_SAFE_RELEASE(healthPool);
    CC_SAFE_RELEASE(fallingObjects);
}

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
    
    screenSize = CCDirector::sharedDirector()->getWinSize();
    running = false;

    createGameScreen();
    createPools();
    createActions();

    fallingObjects = CCArray::createWithCapacity(40);
    fallingObjects->retain();
    

    this->setTouchEnabled(true);
    this->schedule(schedule_selector(GameLayer::update));
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3", true);

    return true;
}

void GameLayer::update(float dt) {
    if (!running) {
        return;
    }

    int count, i;
    CCSprite *sprite;

    meteorTimer += dt;
    if (meteorTimer > meteorInterval) {
        meteorTimer = 0;
        this->resetMeteor();
    }

    healthTimer += dt;
    if (healthTimer > healthInterval) {
        healthTimer = 0;
        this->resetHealth();
    }


    difficultyTimer += dt;
    if (difficultyTimer > difficultyInterval) {
        difficultyTimer = 0;
        this->increaseDifficulty();
    }

    if (bomb->isVisible()) {
        if (bomb->getScale() > 0.3f) {
            if (bomb->getOpacity() != 255) {
                bomb->setOpacity(255);
            }
        }
    }

    if (shockWave->isVisible()) {
        count = fallingObjects->count();
        float diffx, diffy;

        for (i = count - 1; i >= 0; --i) {
            sprite = (CCSprite*) fallingObjects->objectAtIndex(i);
            diffx = shockWave->getPositionX() - sprite->getPositionX();
            diffy = shockWave->getPositionY() - sprite->getPositionY();

            if (pow(diffx, 2) + pow(diffy, 2) <= pow(shockWave->boundingBox().size.width * 0.5f, 2)) {
                sprite->stopAllActions();
                sprite->runAction((CCAction *) explosion->copy()->autorelease());
                SimpleAudioEngine::sharedEngine()->playEffect("boom.wav");


                if (sprite->getTag() == kSpriteMeteor) {
                    shockwaveHits++;
                    score += shockwaveHits * 13 + shockwaveHits * 2;
                }
                fallingObjects->removeObjectAtIndex(i);
            }
        }

        char s[100] = {0};
        sprintf(s, "%i", score);
        scoreDisplay->setString(s);
    }

    count = clouds->count();
    for (i = 0; i < count; ++i) {
        sprite = (CCSprite*) clouds->objectAtIndex(i);
        sprite->setPositionX(sprite->getPositionX() + dt * 20);

        if (sprite->getPositionX() > screenSize.width + sprite->boundingBox().size.width * 0.5f) {
            sprite->setPositionX(-sprite->boundingBox().size.width * 0.5f);
        }
    }
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *event) {

    if (!running) {
        if (introMessage->isVisible()) {
            introMessage->setVisible(false);
        } else if (gameOverMessage->isVisible()) {
            SimpleAudioEngine::sharedEngine()->stopAllEffects();
            gameOverMessage->setVisible(false);
        }
        this->resetGame();
        return;
    }

    CCTouch *touch = (CCTouch *) pTouches->anyObject();
    if (touch) {
        if (bomb->isVisible()) {
            bomb->stopAllActions();
            
            CCSprite *child;
            child = (CCSprite *) bomb->getChildByTag(kSpriteHalo);
            child->stopAllActions();

            child = (CCSprite *) bomb->getChildByTag(kSpriteSparkle);
            child->stopAllActions();
        
            if (bomb->getScale() > 0.3f) {
                shockWave->setScale(0.1f);
                shockWave->setPosition(bomb->getPosition());
                shockWave->setVisible(true);

                shockWave->runAction(CCScaleTo::create(0.5f, bomb->getScale() * 2.0f));
                shockWave->runAction((CCFiniteTimeAction*) shockwaveSequence->copy()->autorelease());

                SimpleAudioEngine::sharedEngine()->playEffect("bombRelease.wav");
            } else {
            
                SimpleAudioEngine::sharedEngine()->playEffect("bombFail.wav");
            }
            bomb->setVisible(false);
            shockwaveHits = 0;
        
        } else {
            CCPoint tap = touch->getLocation();
            bomb->stopAllActions();
            bomb->setScale(0.1f);
            bomb->setPosition(tap);
            bomb->setVisible(true);
            bomb->setOpacity(50);
            bomb->runAction((CCAction *)growBomb->copy()->autorelease());
        
            CCSprite *child;
            child = (CCSprite *) bomb->getChildByTag(kSpriteHalo);
            child->runAction((CCAction *) rotateSprite->copy()->autorelease());
            child = (CCSprite *) bomb->getChildByTag(kSpriteSparkle);
            child->runAction((CCAction *) rotateSprite->copy()->autorelease());
        }
    }
}

void GameLayer::fallingObjectDone(CCNode *pSender) {
    fallingObjects->removeObject(pSender);
    pSender->stopAllActions();
    pSender->setRotation(0);

    if (pSender->getTag() == kSpriteMeteor) {
        energy = 15;
        pSender->runAction((CCAction*) groundHit->copy()->autorelease());
        SimpleAudioEngine::sharedEngine()->playEffect("boom.wav");
    } else {
        pSender->setVisible(false);

        if (energy == 100) {
            score += 25;
            char s[100] = {0};
            sprintf(s, "%i", score);
            scoreDisplay->setString(s);
        } else {
            energy += 10;
            if (energy > 100) {
                energy = 100;
            }
        }
        SimpleAudioEngine::sharedEngine()->playEffect("health.wav");
    }


    if (energy <= 100) {
        energy = 0;
        this->stopGame();
        SimpleAudioEngine::sharedEngine()->playEffect("fire_truck.wav");
        gameOverMessage->setVisible(true);
    }

    char e[100] = {0};
    sprintf(e, "%i%s", energy, "%");
    energyDisplay->setString(e);
}

void GameLayer::animationDone(CCNode *pSender) {
    pSender->setVisible(false);
}

void GameLayer::shockwaveDone() {
    shockWave->setVisible(false);
}

void GameLayer::resetMeteor() {

    if (fallingObjects->count() > 30) {
        return;
    }

    CCSprite *meteor = (CCSprite *) meteorPool->objectAtIndex(meteorPoolIndex);
    meteorPoolIndex++;
    
    if (meteorPoolIndex == meteorPool->count()) {
        meteorPoolIndex = 0;
    }

    int meteorX = rand() % (int) (screenSize.width * 0.8f) + screenSize.width * 0.1f;
    int meteorTargetX = rand() % (int) (screenSize.width * 0.8f) + screenSize.width * 0.1f;

    meteor->stopAllActions();
    meteor->setPosition(ccp(meteorX, screenSize.height + meteor->boundingBox().size.height * 0.5));

    CCActionInterval *rotate = CCRotateBy::create(0.5f, -90);
    CCAction *repeatRotate = CCRepeatForever::create(rotate);

    CCFiniteTimeAction *sequence = CCSequence::create(
        CCMoveTo::create(meteorSpeed, ccp(meteorTargetX, screenSize.height * 0.15f)),
        CCCallFuncN::create(this, callfuncN_selector(GameLayer::fallingObjectDone)),
        NULL);

    meteor->setVisible(true);
    meteor->runAction(repeatRotate);
    meteor->runAction(sequence);
    fallingObjects->addObject(meteor);
}

void GameLayer::resetHealth() {
    if (fallingObjects->count() > 30) {
        return;
    }

    CCSprite *health = (CCSprite*) healthPool->objectAtIndex(healthPoolIndex);
    healthPoolIndex++;

    if (healthPoolIndex == healthPool->count()) {
        healthPoolIndex = 0;
    }

    int healthX = rand() %(int) (screenSize.width * 0.8f) + screenSize.width * 0.1f;
    int healthTargetX = rand() % (int) (screenSize.width * 0.8f) + screenSize.width * 0.1f;

    health->stopAllActions();
    health->setPosition(ccp(healthX, screenSize.height + health->boundingBox().size.height * 0.5));

    CCFiniteTimeAction *sequence = CCSequence::create(
        CCMoveTo::create(healthSpeed, ccp(healthTargetX, screenSize.height * 0.15f)),
        CCCallFuncN::create(this, callfuncN_selector(GameLayer::fallingObjectDone)),
        NULL);

    health->setVisible(true);
    health->runAction((CCAction*) swingHealth->copy()->autorelease());
    health->runAction(sequence);
    fallingObjects->addObject(health);
}

void GameLayer::resetGame() {
    score = 0;
    energy = 100;

    meteorInterval = 2.5;
    meteorTimer = meteorInterval * 0.99f;
    meteorSpeed = 10;

    healthInterval = 20;
    healthTimer = 0;
    healthSpeed = 15;

    difficultyInterval = 60;
    difficultyTimer = 0;

    running = true;

    char e[100] = {0};
    sprintf(e, "%i%s", energy, "%");
    energyDisplay->setString(e);

    char s[100] = {0};
    sprintf(s, "%i", score);
    scoreDisplay->setString(s);
}

void GameLayer::increaseDifficulty() {
    
    meteorInterval -= 0.2f;
    if (meteorInterval < 0.25f) {
        meteorInterval = 0.25f;
    }

    meteorSpeed -= 1;
    if (meteorSpeed < 4) {
        meteorSpeed = 4;
    }

    healthSpeed -= 1;
    if (healthSpeed < 8) {
        healthSpeed = 8;
    }
}

void GameLayer::stopGame() {
    running = false;
    int i, count;
    CCSprite *sprite;

    count = fallingObjects->count();

    for (i = count - 1; i >= 0; --i) {
        sprite = (CCSprite*) fallingObjects->objectAtIndex(i);
        sprite->stopAllActions();
        sprite->setVisible(false);
        fallingObjects->removeObjectAtIndex(i);
    }

    if (bomb->isVisible()) {
        bomb->stopAllActions();
        bomb->setVisible(true);

        CCSprite *child;

        child = (CCSprite*) bomb->getChildByTag(kSpriteHalo);
        child->stopAllActions();

        child = (CCSprite*) bomb->getChildByTag(kSpriteSparkle);
        child->stopAllActions();
    }

    if (shockWave->isVisible()) {
        shockWave->stopAllActions();
        shockWave->setVisible(false);
    }

}

void GameLayer::createGameScreen() {
    CCSprite *bg = CCSprite::create("bg.png");
    bg->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
    this->addChild(bg);

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("sprite_sheet.plist");
    gameBatchNode = CCSpriteBatchNode::create("sprite_sheet.png");
    this->addChild(gameBatchNode);

    CCSprite *sprite;
    // add cityscape
    for (int i = 0; i < 2; ++i) {
        sprite = CCSprite::createWithSpriteFrameName("city_dark.png");
        sprite->setPosition(ccp(screenSize.width * (0.25f + i * 0.5f), sprite->boundingBox().size.height * 0.5f));
        gameBatchNode->addChild(sprite, kForeground);

        sprite = CCSprite::createWithSpriteFrameName("city_light.png");
        sprite->setPosition(ccp(screenSize.width * (0.25f + i * 0.5f), sprite->boundingBox().size.height * 0.9f));
        gameBatchNode->addChild(sprite, kBackground);
    }

    for (int i = 0; i < 3; ++i) {
        sprite = CCSprite::createWithSpriteFrameName("trees.png");
        sprite->setPosition(ccp(screenSize.width * (0.2f + i * 0.3f), sprite->boundingBox().size.height * 0.5));
        gameBatchNode->addChild(sprite, kForeground);
    }

    scoreDisplay = CCLabelBMFont::create("0", "font.fnt", screenSize.width * 0.3f);
    scoreDisplay->setAnchorPoint(ccp(1, 0.5));
    scoreDisplay->setPosition(ccp(screenSize.width * 0.8f, screenSize.height * 0.93f));
    this->addChild(scoreDisplay);

    energyDisplay = CCLabelBMFont::create("100%", "font.fnt", screenSize.width * 0.1f, kCCTextAlignmentRight);
    energyDisplay->setPosition(ccp(screenSize.width * 0.3f, screenSize.height * 0.94f));
    this->addChild(energyDisplay);

    CCSprite *icon = CCSprite::createWithSpriteFrameName("health_icon.png");
    icon->setPosition(ccp(screenSize.width * 0.15f, screenSize.height * 0.94f));
    gameBatchNode->addChild(icon, kBackground);

    CCSprite *cloud;
    clouds = CCArray::createWithCapacity(4);
    clouds->retain();
    float cloudY;
    for (int i = 0; i < 4; ++i) {
        cloudY = i % 2 == 0 ? screenSize.height * 0.4f : screenSize.height * 0.5f;
        cloud = CCSprite::createWithSpriteFrameName("cloud.png");
        cloud->setPosition(ccp(screenSize.width * 0.1f + i * screenSize.width * 0.3f, cloudY));
        gameBatchNode->addChild(cloud, kBackground);
        clouds->addObject(cloud);
    }

    bomb = CCSprite::createWithSpriteFrameName("bomb.png");
    bomb->getTexture()->generateMipmap();
    bomb->setVisible(false);

    CCSize size = bomb->boundingBox().size;

    CCSprite *sparkle = CCSprite::createWithSpriteFrameName("sparkle.png");
    sparkle->setPosition(ccp(screenSize.width * 0.72f, screenSize.height * 0.72f));
    bomb->addChild(sparkle, kMiddleground, kSpriteSparkle);

    CCSprite *halo = CCSprite::createWithSpriteFrameName("halo.png");
    halo->setPosition(ccp(size.width * 0.4f, size.height * 0.4f));
    bomb->addChild(halo, kMiddleground, kSpriteHalo);
    gameBatchNode->addChild(bomb, kForeground);

    shockWave = CCSprite::createWithSpriteFrameName("shockwave.png");
    shockWave->getTexture()->generateMipmap();
    shockWave->setVisible(false);
    gameBatchNode->addChild(shockWave);

    introMessage = CCSprite::createWithSpriteFrameName("logo.png");
    introMessage->setPosition(ccp(screenSize.width * 0.5f, screenSize.height * 0.6f));
    introMessage->setVisible(true);
    this->addChild(introMessage, kForeground);

    gameOverMessage = CCSprite::createWithSpriteFrameName("gameover.png");
    gameOverMessage->setPosition(ccp(screenSize.width * 0.5, screenSize.height * 0.65));
    gameOverMessage->setVisible(false);
    this->addChild(gameOverMessage, kForeground);
}

void GameLayer::createPools() {
    CCSprite *sprite;
    int i;

    meteorPool = CCArray::createWithCapacity(50);
    meteorPool->retain();
    meteorPoolIndex = 0;

    for (i = 0; i < 50; ++i) {
        sprite = CCSprite::createWithSpriteFrameName("meteor.png");
        sprite->setVisible(false);
        gameBatchNode->addChild(sprite, kMiddleground, kSpriteMeteor);
        meteorPool->addObject(sprite);
    }


    healthPool = CCArray::createWithCapacity(20);
    healthPool->retain();
    healthPoolIndex = 0;
    for (i = 0; i < 20; ++i) {
        sprite = CCSprite::createWithSpriteFrameName("health.png");
        sprite->setVisible(false);
        sprite->setAnchorPoint(ccp(0.5f, 0.5f));
        gameBatchNode->addChild(sprite, kMiddleground, kSpriteHealth);
        healthPool->addObject(sprite);
    } 

}

void GameLayer::createActions() {
    CCFiniteTimeAction *easeSwing = CCSequence::create(
        CCEaseInOut::create(CCRotateTo::create(1.2f, -10), 2),
        CCEaseInOut::create(CCRotateTo::create(1.2f,  10), 2),
        NULL);

    swingHealth = CCRepeatForever::create((CCActionInterval*) easeSwing);
    swingHealth->retain();

    shockwaveSequence = CCSequence::create(
        CCFadeOut::create(1.0f),
        CCCallFunc::create(this, callfunc_selector(GameLayer::shockwaveDone)), NULL);

    shockwaveSequence->retain();

    growBomb = CCScaleTo::create(6.0f, 1);
    growBomb->retain();

    CCActionInterval *rotate = CCRotateBy::create(0.5f, -90);
    rotateSprite = CCRepeatForever::create(rotate);
    rotateSprite->retain();

    CCAnimation *animation = CCAnimation::create();
    CCSpriteFrame *frame;
    int i;
    for (int i = 1; i <= 10; ++i) {
        char name[100] = {0};
        sprintf(name, "boom%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(1 / 10.0f);
    animation->setRestoreOriginalFrame(true);

    groundHit = CCSequence::create(
        CCMoveBy::create(0, ccp(0, screenSize.height * 0.12f)),
        CCAnimate::create(animation),
        CCCallFuncN::create(this, callfuncN_selector(GameLayer::animationDone)),
        NULL);

    groundHit->retain();

    animation = CCAnimation::create();
    for (i = 1; i <= 7; ++i) {
        char e[100] = {0};
        sprintf(e, "explosion_small%i.png", i);
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(e);
        animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(1 / 10.0f);
    animation->setRestoreOriginalFrame(true);

    explosion = CCSequence::create(
        CCAnimate::create(animation),
        CCCallFuncN::create(this, callfuncN_selector(GameLayer::animationDone)), 
        NULL);

    explosion->retain();   
}
