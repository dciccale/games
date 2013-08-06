#include "GameLayer.h"
#include "Bullet.h"

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
	
    time(&origTime);

	screenSize = CCDirector::sharedDirector()->getWinSize();

	const char* backgroundImg = "Background.png";
	background = Background::createBackground(backgroundImg);
	this->addChild(background);
    
	const char* shipImg = "Ship.png";
	ship = Ship::createShip(shipImg);
	this->addChild(ship);
	CCSize shipSize = ship->getTextureRect().size;
	ship->setPosition(ccp(shipSize.width / 2, (screenSize.height - shipSize.height) / 2));
	
	// TODO: release in destructor
	shipBullets = new CCArray();
	shipBullets->init();

	// TODO: release in destructor
	enemies = new CCArray();
	enemies->init();

	// TODO: release in destructor
	enemyBullets = new CCArray();
	enemyBullets->init();

	this->setTouchEnabled(true);
	this->schedule(schedule_selector(GameLayer::update));    
    return true;
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *event) {
	CCSetIterator it;
	CCTouch *touch;
	CCPoint tap;
	for (it = pTouches->begin(); it != pTouches->end(); ++it) {
		touch = (CCTouch*) (*it);
		if (touch) {
			tap = touch->getLocation();
			if (ship->boundingBox().containsPoint(tap)) {
				ship->setTouch(touch);
				// shoot ... (refactor, the ship class needs a shoot method (with direction, etc...))
				int bullets = shipBullets->count();
				if (bullets < 12) {
					Bullet *bullet = Bullet::createBullet();
					bullet->setVelocity(ccp(0.3f + CCRANDOM_0_1() * 5.0f, 0));
					bullet->setPositionX(ship->getPositionX() + ship->getTextureRect().size.width / 2);
					bullet->setPositionY(ship->getPositionY());
					shipBullets->addObject(bullet);
					this->addChild(bullet);
				}
			}
		}
	}
}

void GameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *event) {
	CCSetIterator it;
	CCTouch *touch;
	CCPoint tap;
	for (it = pTouches->begin(); it != pTouches->end(); ++it) {
		touch = (CCTouch*) (*it);
		if (touch) {
			tap = touch->getLocation();
			if (ship->getTouch() != NULL && ship->getTouch() == touch) {
				ship->setScale(1.2f);
				ship->setPosition(tap);
			}
		}
	}
}

void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *event) {
	CCSetIterator it;
	CCTouch *touch;
	CCPoint tap;
	for (it = pTouches->begin(); it != pTouches->end(); ++it) {
		touch = (CCTouch*) (*it);
		if (touch) {
			tap = touch->getLocation();
			if (ship->getTouch() != NULL && ship->getTouch() == touch) {
				ship->setScale(1.0f);
				ship->setTouch(NULL);
			}
		}
	}
}

void GameLayer::updateShip() {
	// check ship position (keep it on the screen!)
	CCSize shipSize = ship->getTextureRect().size;
	float w = shipSize.width / 2;
	float h = shipSize.height / 2;
	CCPoint pos = ship->getPosition();

	if (pos.x + w > screenSize.width) {
		ship->setPositionX(screenSize.width - w);
	} else if (pos.x < w) {
		ship->setPositionX(w);
	}
	
	if (pos.y + h > screenSize.height) {
		ship->setPositionY(screenSize.height - h);
	} else if (pos.y < h) {
		ship->setPositionY(h);
	}

	// move ship bullets
	CCObject *sb;
	CCARRAY_FOREACH(shipBullets, sb) {
		Bullet *bullet = dynamic_cast<Bullet*>(sb);
		bullet->setPositionX(bullet->getPositionX() + bullet->getVelocity().x);

		CCObject *en;
		CCARRAY_FOREACH(enemies, en) {
			Ship *enemy = dynamic_cast<Ship*>(en);
			if (enemy->boundingBox().containsPoint(bullet->getPosition())) {
				enemy->setScale(0.75f);
				enemy->setHealth(enemy->getHealth() - 1);

				if (enemy->getHealth() < 0) {
					enemies->removeObject(enemy);
					this->removeChild(enemy);
				}
			} else {
				enemy->setScale(1.0f);
			}
		}

		if (bullet->getPositionX() > screenSize.width + 5) {
			shipBullets->removeObject(bullet);
			this->removeChild(bullet);
		}
	}

	// check health
	if (ship->getHealth() > 0) {
		ship->setHealth(ship->getHealth() - 1);
	} else if (ship->getHealth() < 0) {
		ship->setHealth(0);
		// game over ...
	}
}

void GameLayer::updateEnemies() {
	CCObject *en;

	CCARRAY_FOREACH(enemies, en) {
		Ship *enemy = dynamic_cast<Ship*>(en);
		CCPoint angle = enemy->getAngle();
		CCPoint pos = enemy->getPosition(); 
		CCPoint range = enemy->getRange();
		CCPoint speed = enemy->getSpeed();
		CCPoint vel = enemy->getVelocity();
		CCSize enemySize = enemy->getTextureRect().size;
		float w = enemySize.width / 2;
		float h = enemySize.height / 2;

		enemy->setPositionX(pos.x - vel.x);
		enemy->setPositionY(pos.y - sin(angle.y) * range.y);
		enemy->setAngle(ccp(angle.x, angle.y + speed.y));


		if (ship->boundingBox().containsPoint(enemy->getPosition())) {
			ship->setHealth(ship->getHealth() - 1);
		}
		if (pos.x < (w * -1)) {
			enemies->removeObject(enemy);
			break;
		}
	}

	// move enemy bullets
	CCObject *eb;
	CCARRAY_FOREACH(enemyBullets, eb) {
		Bullet *bullet = dynamic_cast<Bullet*>(eb);

		bullet->setPositionX(bullet->getPositionX() - bullet->getVelocity().x);

		if (ship->boundingBox().containsPoint(bullet->getPosition())) {
			ship->setHealth(ship->getHealth() - 1);
		}
		
		if (bullet->getPositionX() < -5) {
			enemyBullets->removeObject(bullet);
			this->removeChild(bullet);
			break;
		}
	}
}

void GameLayer::addEnemies() {
	
	if (enemies->count() < 10) {
		float w1 = screenSize.width / 2;
		float h1 = screenSize.height / 2;
		
		const char* enemyImg = "Enemy.png";
		Ship* enemy = Ship::createShip(enemyImg);
		CCSize size = enemy->getTextureRect().size;
		float w2 = size.width / 2;
		float h2 = size.height / 2;
		enemy->setHealth(30);
		enemy->setPositionX(screenSize.width + w2);
		enemy->setPositionY(CCRANDOM_0_1() * screenSize.height);

		// fix y position just in case we're too high or low
		float randY = 60 + CCRANDOM_0_1() * 200 - 100;
		if (enemy->getPositionY() < h1 - randY) {
			enemy->setPositionY(h1 - randY);
		} else if (enemy->getPositionY() > h1 + randY) {
			enemy->setPositionY(h1 + randY);
		}

		float vx, vy;
		vx = 0.2f + CCRANDOM_0_1() * (0.5f - 0.2f);
		vy = (CCRANDOM_0_1() * 1.0f < 1.0f) ? - 0.15f : 0.15f;

		enemy->setAngle(ccp(0, 0));
		enemy->setRange(ccp(2, 2));
		enemy->setSpeed(ccp(0.15f, 0.15f));
		enemy->setVelocity(ccp(vx, vy));
		
		enemies->addObject(enemy);
		this->addChild(enemy);

		// add bullets 
		int n = 3.0 + CCRANDOM_0_1() * 5.0f - 2.0f;
		if (n < 3) {
			n = 3;
		}

		for (int i = 0; i < n; ++i) {
			Bullet *bullet = Bullet::createBullet();
			bullet->setVelocity(ccp(1.0 + CCRANDOM_0_1() * 1.0 - 0.5, 0));
			bullet->setPosition(ccp(enemy->getPositionX() - w2, enemy->getPositionY()));

			enemyBullets->addObject(bullet);
			this->addChild(bullet);
		}

	}
}

void GameLayer::update(float dt) {

	background->scroll();
	this->updateShip();

	// some methods will be invoked every ~5 seconds
	time_t currTime;
	time(&currTime);
	double seconds = difftime(currTime, origTime);
	if (seconds > 4) {
		time(&origTime);
		this->addEnemies();
	}
	this->updateEnemies();
}
