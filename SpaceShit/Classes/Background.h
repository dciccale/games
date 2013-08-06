#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "cocos2d.h"

using namespace cocos2d;

class Background : public CCSpriteBatchNode {
public:
	CC_SYNTHESIZE(float, speed, Speed);

	Background(void);
	static Background* createBackground(const char *fileImage);
	void scroll(); 
};

#endif;