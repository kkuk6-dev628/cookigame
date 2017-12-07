#pragma once
#include "2d/CCNode.h"
#include "2d/CCSprite.h"

class ShowObject : public cocos2d::Node
{
public:
	ShowObject();
	~ShowObject();

	CREATE_FUNC(ShowObject)

	void setSpriteFrame(cocos2d::SpriteFrame* spriteFrame);

protected:
	cocos2d::Sprite* textureSprite = nullptr;
};

