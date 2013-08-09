#ifndef __SCORELAYER_H__
#define __SCORELAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class ScoreLayer : public CCLayer
{
    int _score;
    float _interval;
    char _text[32];
    void updateText();
    
public:
    ScoreLayer();
    
    CCLabelTTF* _label;
    int getScore();
    void update(float dt);
    
    float getInterval();
    void setInterval(float i);
};

#endif
