#include "LavaCakeObject.h"
#include "Models/BoardModels/Cell.h"
#include "Controllers/BoardController.h"

void LavaCakeObject::setIncreaseLavaCakeFlag(bool flag)
{
	increaseLevel = flag;
}

void LavaCakeObject::addLayers(char layerCount)
{
	layers += layerCount;
	initTexture();
}

LavaCakeObject::LavaCakeObject()
	:LargeTile()
{
	receiveNearbyAffect = true;
	noClear = true;
	canMatch = false;
	colspan = 2;
	rowspan = 2;
}


LavaCakeObject::~LavaCakeObject()
{
}

void LavaCakeObject::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	initTexture();
}


void LavaCakeObject::initTexture()
{
	auto textureName = StringUtils::format("cake_%d.png", lavaLevel);
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

bool LavaCakeObject::crush(bool showEffect)
{
	if (!increaseLevel) return false;

	increaseLevel = false;
	auto ret = false;
	if(lavaLevel < 4)
	{
		lavaLevel++;
		ret = false;
	}
	else
	{
		lavaLevel = 1;
		ret = true;
	}

	auto effect = poolController->getLavaCakeEffect();
	effect->setPosition(Vec2(CellSize, CellSize * 0.5));
	addChild(effect);

	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->schedule([=](float dt)
	{
		this->initTexture();
	}, this, 1.0f, false, "lavacake");

	return ret;
}
