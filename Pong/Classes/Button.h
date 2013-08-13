#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "cocos2d.h"
USING_NS_CC;

class Button : public CCSprite {
    const char *labelText;
    CCLabelTTF *labelTTF;

public:
    Button();
    static Button* createButton(const char *label);
    virtual void draw();
    void setLabel(const char *label);
    const char *getLabel();
};

#endif