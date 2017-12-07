#include "ShowObject.h"
#include "General/Constants.h"


ShowObject::ShowObject()
{
	setContentSize(cocos2d::Size(CellSize, CellSize));
	setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
}


ShowObject::~ShowObject()
{
}

void ShowObject::setSpriteFrame(cocos2d::SpriteFrame* spriteFrame)
{
	if(textureSprite == nullptr)
	{
		textureSprite = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);
		textureSprite->setContentSize(cocos2d::Size(CellSize, CellSize));
		textureSprite->setPosition(CellSize / 2, CellSize / 2);
		textureSprite->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
		addChild(textureSprite);
	}
	else
	{
		textureSprite->setSpriteFrame(spriteFrame);
	}
}
