#include "BoardLayer.h"
#include "General/Constants.h"
#include "2d/CCDrawNode.h"


void BoardLayer::initWithGrid(const char col, const char row, bool drawBorder)
{
	const auto width = col * CellSize;
	const auto height = row * CellSize;
	setContentSize(cocos2d::Size(width, height));
	const auto originX = CenterX - width / 2;
	const auto originY = CenterY - height / 2;
	setPositionX(0);
	setPositionY(0);
	setAnchorPoint(cocos2d::Vec2(0, 0));

	if (drawBorder) {
		auto rectNode = cocos2d::DrawNode::create();
		cocos2d::Vec2 rectangle[4];
		rectangle[0] = cocos2d::Vec2(0, 0);
		rectangle[1] = cocos2d::Vec2(0, height);
		rectangle[2] = cocos2d::Vec2(width, height);
		rectangle[3] = cocos2d::Vec2(width, 0);
		cocos2d::Color4F white(1, 1, 1, 1);
		rectNode->drawPolygon(
			rectangle,
			4,
			cocos2d::Color4F(0, 0, 0, 0),
			2,
			cocos2d::Color4F(cocos2d::Color4B::WHITE));
		rectNode->setPosition(0, 0);
		addChild(rectNode);
	}
}
