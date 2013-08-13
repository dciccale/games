#include "Button.h"

Button::Button() {

}

Button* Button::createButton(const char *label) {
    Button *sprite = new Button();

    if (sprite && sprite->init()) {
        sprite->autorelease();
        sprite->setLabel(label);

        
       
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

const char* Button::getLabel() {
    return labelText;
}

void Button::setLabel(const char* label) {
    labelText = label;
}

void Button::draw() {
    labelTTF = CCLabelTTF::create(label, "Arial", 12);
}