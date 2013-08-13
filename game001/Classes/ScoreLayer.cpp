#include "ScoreLayer.h"

USING_NS_CC;

ScoreLayer::ScoreLayer()
{
    _score = 0;
    _interval = 1.0f;
    this->updateText();
    _label = CCLabelTTF::create(_text, "arial", 32);
}

int ScoreLayer::getScore()
{
    return _score;
}

void ScoreLayer::update(float dt)
{
    _score++;
    if (_score == 0) {
        CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
    }
    this->updateText();
    _label->setString(_text);
}

void ScoreLayer::updateText()
{
    sprintf(_text, "SCORE: %d", _score);
}

float ScoreLayer::getInterval()
{
    return _interval;
}

void ScoreLayer::setInterval(float i)
{
    _interval = i;
}