#include "ProgressBar.h"

USING_NS_CC;

ProgressBar::ProgressBar(const char* backImage, const char* foreImage)
{
	Sprite::initWithFile(backImage);
	Size sz = getContentSize();

	Sprite* sprite = Sprite::create(foreImage);
	_progress = ProgressTimer::create(sprite);
	_progress->setType(ProgressTimer::Type::BAR);
	_progress->setMidpoint(Point(1, 0.5f));
	_progress->setBarChangeRate(Point(1, 0));
	_progress->setPosition(Point(sz.width/2, sz.height/2));
	addChild(_progress);
}

ProgressBar::~ProgressBar()
{
}
