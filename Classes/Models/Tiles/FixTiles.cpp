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
	auto tempDirStr = direction._to_string();
	if(direction == +Direction::right)
	{
		tempDirStr = ((Direction)Direction::left)._to_string();
	}
	else if(direction == +Direction::up)
	{
		tempDirStr = ((Direction)Direction::down)._to_string();
	}
	if(direction == +Direction::any)
	{
		tempDirStr = dirString.c_str();
	}
	auto textureName = StringUtils::format("%s_%s.png", type.c_str(), tempDirStr);
	TileBase::initTexture(textureName);
}

void PieceSwapperObject::initTexture()
{
	auto textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
	TileBase::initTexture(textureName);
}

void PathFollowerObject::initTexture()
{
	rootNode = cocos2d::CSLoader::createNode("res/skeletal/SpinePatherIceCream.csb");
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

void PortalInletObject::initTexture()
{
	auto textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
	TileBase::initTexture(textureName);
	textureSprite->setContentSize(Size(CellSize, PORTALLETHEIGHT));
	
	if (direction == +Direction::S)
	{
		textureSprite->setPosition(Vec2(CellSize / 2, -PORTALLETHEIGHT / 2.f));
	}
	else
	{
		textureSprite->setPosition(Vec2(CellSize / 2, CellSize + PORTALLETHEIGHT / 2.f));
	}
}

void PortalInletObject::setInWater()
{
	textureSprite->setPosition(Vec2(CellSize / 2, CellSize + PORTALLETHEIGHT / 2.f));
}

void PortalOutletObject::initTexture()
{
	auto textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
	TileBase::initTexture(textureName);
	textureSprite->setContentSize(Size(CellSize, PORTALLETHEIGHT));

	if (direction == +Direction::N)
	{
		textureSprite->setPosition(Vec2(CellSize / 2, CellSize + PORTALLETHEIGHT / 2.f));
	}
	else
	{
		textureSprite->setPosition(Vec2(CellSize / 2, -PORTALLETHEIGHT / 2.f));
	}
}

void PortalOutletObject::setInWater()
{
	textureSprite->setPosition(Vec2(CellSize / 2, -PORTALLETHEIGHT / 2.f));
}
