#include "FruitRollObject.h"
#include "FruitRollGroup.h"
#include "Models/BoardModels/Cell.h"
#include "ThopplerTile.h"

FruitRollObject::FruitRollObject()
{
	receiveNearbyAffect = true;
	
}


FruitRollObject::~FruitRollObject()
{
}

void FruitRollObject::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	if(positionString == "origin")
	{
		isHead = true;
	}
	initTexture();
}

bool FruitRollObject::crush(bool showEffect)
{
	auto removedCell = group->rolling();
	if(removedCell != nullptr)
	{
		auto thopplerTile = removedCell->getTileAtLayer(LayerId::Toppling);
		if (thopplerTile != nullptr)
		{
			static_cast<ThopplerTile*>(thopplerTile)->moveDown();
			removedCell->setSourceTile(thopplerTile);
			removedCell->removeTileAtLayer(LayerId::Toppling);
		}
		else
		{
			removedCell->clear();
		}
	}
	return false;
}

void FruitRollObject::initTexture()
{
	std::string textureName = "FruitRollObject.png";
	auto rotation = 0.f;

	if(dirString == "down" || dirString == "up")
	{
		rotation = 270;
	}
	else if (dirString == "down-right")
	{
		textureName = "FruitRollObject_2.png";
	}
	else if (dirString == "down-left")
	{
		textureName = "FruitRollObject_1.png";
	}
	else if (dirString == "left-down")
	{
		textureName = "FruitRollObject_3.png";
	}
	else if (dirString == "left-up")
	{
		textureName = "FruitRollObject_2.png";
	}
	else if (dirString == "right-down")
	{
		textureName = "FruitRollObject_4.png";
	}
	else if (dirString == "up-left")
	{
		textureName = "FruitRollObject_4.png";
	}
	else if (dirString == "up-right")
	{
		textureName = "FruitRollObject_3.png";
	}
	else if (dirString == "right-up")
	{
		textureName = "FruitRollObject_1.png";
	}

	if (positionString == "origin")
	{
		textureName = "FruitRollObject_Head.png";
	}
	initTexture(textureName);
	textureSprite->setRotation(rotation);
	if (positionString == "origin")
	{
		if (dirString == "left")
		{
			textureSprite->setScaleX(-1);
		}
		else if (dirString == "down")
		{
			textureSprite->setScaleX(-1);
		}
	}
}

void FruitRollObject::setTextureRotation(Node* node)
{
	auto rotation = 0;
	auto scaleX = 1;
	Vec2 pos;
	if (dirString == "up" || dirString == "up-left" || dirString == "up-right")
	{
		pos.x = CellSize / 2;
		pos.y = 0;
		scaleX = -1;
	}
	else if (dirString == "down-right" || dirString == "down-left" || dirString == "down")
	{
		pos.x = CellSize / 2;
		pos.y = CellSize;
		rotation = 180;
	}
	else if (dirString == "left" || dirString == "left-down" || dirString == "left-up")
	{
		pos.x = CellSize;
		pos.y = CellSize / 2;
		rotation = 270;
	}
	else if (dirString == "right" || dirString == "right-down" || dirString == "right-up")
	{
		pos.x = 0;
		pos.y = CellSize / 2;
		rotation = 90;
		scaleX = -1;
	}
	node->setRotation(rotation);
	auto currentScale = node->getScaleX();
	node->setScaleX(scaleX * currentScale);
	node->setPosition(pos);
}

void FruitRollObject::setOneFruitTexture()
{
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("FruitRollObject_One.png");
	if (textureSprite == nullptr)
	{
		textureSprite = Sprite::createWithSpriteFrame(spriteFrame);
		addChild(textureSprite);
	}
	else
	{
		textureSprite->setSpriteFrame(spriteFrame);
	}
	textureSprite->setVisible(true);
	textureSprite->setAnchorPoint(Vec2(0.5, 0.5));
	textureSprite->setPosition(CellSize / 2, CellSize / 2);
	textureSprite->setContentSize(Size(CellSize, CellSize));
}

char FruitRollObject::getRemainderCount()
{
	auto count = group->getRmainderCount(this);
	return count;
}

void FruitRollObject::initTexture(std::string textureName)
{
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);
	if (spriteFrame == nullptr)
	{
		return;
	}
	if (textureSprite == nullptr)
	{
		textureSprite = Sprite::createWithSpriteFrame(spriteFrame);
		addChild(textureSprite);
	}
	else
	{
		textureSprite->setSpriteFrame(spriteFrame);
	}
	textureSprite->setAnchorPoint(Vec2(0.5, 0.5));
	textureSprite->setPosition(CellSize / 2, CellSize / 2);
	textureSprite->setContentSize(Size(CellSize, CellSize));

}
