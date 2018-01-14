#include "FixTiles.h"


FixTiles::FixTiles()
{
	canMatch = false;
}


//FixTiles::~FixTiles()
//{
//}

void FixTiles::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	initTexture();
}

void FixTiles::initTexture()
{
	canMatch = false;
	std::string textureName;
	if(layers != -1)
	{
		textureName = StringUtils::format("%s_%d.png", type.c_str(), layers);
	}
	else
	{
		textureName = StringUtils::format("%s.png", type.c_str());
	}
	TileBase::initTexture(textureName);
}

void PathObject::initTexture()
{
	canMatch = false;
	std::string textureName;
	if (!direction.empty())
	{
		auto tempDirStr = direction;
		if(direction == "right")
		{
			tempDirStr = "left";
		}
		else if(direction == "up")
		{
			tempDirStr = "down";
		}
		textureName = StringUtils::format("%s_%s.png", type.c_str(), tempDirStr.c_str());
	}
	else
	{
		textureName = StringUtils::format("%s.png", type.c_str());
	}
	TileBase::initTexture(textureName);
}

InvisibleBrickObject::InvisibleBrickObject()
{
	canMatch = false;
	receiveNearbyAffect = false;
}

void ChocolateObject::showCrushEffect()
{
	auto animationShow = poolController->getChocolateCrushShow();
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	//animationShow->setScale(1.5);
	if (getParent() != nullptr) getParent()->addChild(animationShow, 500);
}