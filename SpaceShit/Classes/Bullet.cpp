#include "Bullet.h"

Bullet::Bullet(void) {
  velocity = ccp(0, 0);
}

Bullet *Bullet::createBullet() {
	Bullet *sprite = new Bullet();
	if (sprite != NULL) {
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Bullet::draw() {	
	CCPoint pos = this->getPosition();

	glLineWidth(2);
	ccDrawColor4F(0.0f, 0.0f, 0.0f, 1.0f);
	ccDrawLine(ccp(0, 0), ccp(2, 0));
}
