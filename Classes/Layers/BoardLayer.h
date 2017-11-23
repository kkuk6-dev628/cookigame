#pragma once
#include "2d/CCLayer.h"

class BoardLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(BoardLayer);
	void initWithGrid(char col, char row);
};

