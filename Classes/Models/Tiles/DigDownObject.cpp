#include "DigDownObject.h"
#include "Models/BoardModels/Cell.h"


DigDownObject::DigDownObject()
{
}


DigDownObject::~DigDownObject()
{

}

void DigDownObject::initTexture()
{
	auto spriteName = StringUtils::format("%s_%d.png", type.c_str(), layers);
	TileBase::initTexture(spriteName);
}

//void DigDownObject::initTexture(std::string textureName)
//{
//	
//}

void DigDownObject::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	initTexture();
}

bool DigDownObject::crush(bool showEffect)
{
	layers--;
	if(showEffect)
	{
		showCrushEffect();
	}
	if(layers > 0)
	{
		initTexture();
		return false;
	}
	else
	{
		removeFromParent();
		pCell->removeTileAtLayer(LayerId::Cover);
		return true;
	}
}

void DigDownObject::showCrushEffect()
{
	auto animationShow = poolController->getDigdownCrushShow();
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	//animationShow->setScale(1.5);
	if (getParent() != nullptr) getParent()->addChild(animationShow, 500);
}
