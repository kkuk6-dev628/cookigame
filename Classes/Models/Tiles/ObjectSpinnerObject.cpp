#include "ObjectSpinnerObject.h"
#include "Models/BoardModels/Cell.h"


ObjectSpinnerObject::ObjectSpinnerObject()
{
	receiveNearbyAffect = false;
	noClear = false;
	canMatch = false;
	colspan = 2;
	rowspan = 2;
}


ObjectSpinnerObject::~ObjectSpinnerObject()
{
}

void ObjectSpinnerObject::initTexture()
{
	auto textureName = StringUtils::format("ObjectSpinnerObject_%s.png", dirString.c_str());
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);
	if (spriteFrame == nullptr)
	{
		return;
	}
	if (textureSprite == nullptr)
	{
		textureSprite = Sprite::create();
		addChild(textureSprite);
	}
	textureSprite->setSpriteFrame(spriteFrame);
	textureSprite->setContentSize(Size(CellSize * colspan, CellSize * rowspan));
	textureSprite->setAnchorPoint(Vec2(0, 1));
	textureSprite->setPosition(0, CellSize);
}

void ObjectSpinnerObject::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	initTexture();
}
