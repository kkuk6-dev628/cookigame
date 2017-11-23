#include "BoardLayer.h"
#include "General/Constants.h"


void BoardLayer::initWithGrid(const char col, const char row)
{
	const auto width = col * CellSize;
	const auto height = row * CellSize;
	setContentSize(cocos2d::Size(width, height));
	const auto originX = CenterX - width / 2;
	const auto originY = CenterY - height / 2;
	setPositionX(0);
	setPositionY(0);
	setAnchorPoint(cocos2d::Vec2(0, 0));
}
