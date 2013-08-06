#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

using namespace cocos2d;

class Bullet : public CCSprite {
public:
  CC_SYNTHESIZE(CCPoint, velocity, Velocity);
  Bullet(void);
  static Bullet* Bullet::createBullet();
  virtual void draw();
};
#endif
