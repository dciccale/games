#include "Background.h"

Background::Background(void) { 
	speed = 0.5;
}

Background* Background::createBackground(const char* fileImage) {
	
	Background *sprite = new Background();
	int capacity = 2;

	if (sprite != NULL && sprite->initWithFile(fileImage, capacity)) {
		sprite->autorelease();

		CCTexture2D *texture = sprite->getTexture();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
		CCSize textureSize = texture->getContentSize();
		
		CCRect rect = CCRectZero;
		rect.setRect(0, 0, screenSize.width, screenSize.height);

		for (int i = 0; i < capacity; ++i) {	
			CCSprite *child = CCSprite::createWithTexture(texture, rect);
			child->setTag(i);
			child->setAnchorPoint(ccp(0, 0));
			child->setPositionX(child->getTextureRect().getMaxX() * i);
			sprite->addChild(child);
		}
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}

void Background::scroll() {
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCSize textureSize = this->getTexture()->getContentSize();
	CCSprite *child1 = dynamic_cast<CCSprite*>(this->getChildByTag(0));
	CCSprite *child2 = dynamic_cast<CCSprite*>(this->getChildByTag(1));

	child1->setPositionX(child1->getPositionX() - speed);
	child2->setPositionX(child2->getPositionX() - speed);

	if (child1->getPositionX() < (child1->getTextureRect().size.width * -1)) {
		child1->setPositionX(child2->getPositionX() + child2->getTextureRect().size.width);
	}
	if (child2->getPositionX() < (child2->getTextureRect().size.width * -1)) {
		child2->setPositionX(child1->getPositionX() + child1->getTextureRect().size.width);
	}
}