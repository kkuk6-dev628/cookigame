#include "FixTiles.h"
#include "cocostudio/ActionTimeline/CSLoader.h"


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
	std::string textureName;
	//if (!direction.empty())
	//{
		auto tempDirStr = direction;
		if(direction == +Direction::right)
		{
			tempDirStr = Direction::left;
		}
		else if(direction == +Direction::up)
		{
			tempDirStr = Direction::down;
		}
		textureName = StringUtils::format("%s_%s.png", type.c_str(), tempDirStr._to_string());
	//}
	//else
	//{
	//	textureName = StringUtils::format("%s.png", type.c_str());
	//}
	TileBase::initTexture(textureName);
}

void PathFollowerObject::initTexture()
{
	auto rootNode = cocos2d::CSLoader::createNode("res/skeletal/SpinePatherIceCream.csb");
	//rootNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	rootNode->setContentSize(Size(CellSize, CellSize));
	rootNode->setScale(0.5f);
	rootNode->setPosition(Vec2(CellSize / 2.f, 0));
	addChild(rootNode);

	auto action = cocos2d::CSLoader::createTimeline("res/skeletal/SpinePatherIceCream.csb");
	action->gotoFrameAndPlay(0, true);
	rootNode->runAction(action);
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

bool ChocolateObject::crush(bool showEffect)
{
	if(showEffect) showCrushEffect();
	layers--;
	if(layers > 0)
	{
		initTexture();
		return false;
	}
	else
	{
		return CookieTile::crush(showEffect);
	}
}

void ChocolateObject::initTexture()
{
	if (layers > 0)
	{
		auto textureName = StringUtils::format("%s_%d.png", type.c_str(), layers);
		TileBase::initTexture(textureName);
	}
}

void PortalInletObject::initTexture()
{
	auto textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
	TileBase::initTexture(textureName);
	textureSprite->setContentSize(Size(CellSize, PORTALLETHEIGHT));
	textureSprite->setPosition(Vec2(CellSize / 2, -PORTALLETHEIGHT / 2.f));
}

void PortalOutletObject::initTexture()
{
	auto textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
	TileBase::initTexture(textureName);
	textureSprite->setContentSize(Size(CellSize, PORTALLETHEIGHT));
	textureSprite->setPosition(Vec2(CellSize / 2, CellSize + PORTALLETHEIGHT / 2.f));
}
