#include "LayeredCrackerTile.h"


void LayeredCrackerTile::showCrushEffect()
{
	auto animationShow = poolController->getChocolateCrushShow();
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	//animationShow->setScale(1.5);
	if (getParent() != nullptr) getParent()->addChild(animationShow, 500);
}

bool LayeredCrackerTile::crush(bool showEffect)
{
	if (showEffect) showCrushEffect();
	layers--;
	if (layers > 0)
	{
		initTexture();
		return false;
	}
	else
	{
		return CookieTile::crush(showEffect);
	}
}

void LayeredCrackerTile::initTexture()
{
	if (layers > 0)
	{
		auto textureName = StringUtils::format("%s_%d.png", type.c_str(), layers);
		TileBase::initTexture(textureName);
	}
}

void LayeredCrackerTile::addLayers(char layerCount)
{
	layers += layerCount;
	if (layers > 6)
	{
		layers = 6;
	}
	initTexture();
}

void IceCoverObject::showCrushEffect()
{
	auto animationShow = poolController->getIceCrushShow();
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	//animationShow->setScale(1.5);
	if (getParent() != nullptr) getParent()->addChild(animationShow, 500);
}