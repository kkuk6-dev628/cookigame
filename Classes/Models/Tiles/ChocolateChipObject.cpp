#include "ChocolateChipObject.h"
#include "Models/BoardModels/Cell.h"
#include "Controllers/PoolController.h"


#pragma region ChocolateChipObject

ChocolateChipObject::ChocolateChipObject()
	:MovingTile()
{
	canMatch = false;
	receiveNearbyAffect = true;
}

bool ChocolateChipObject::crush(bool showEffect)
{
	layers--;
	if (showEffect) showCrushEffect();
	if (layers == 0)
	{
		//pCell->afterTileCrushProc();
		poolController->recycleCookieTile(this);
		return true;
	}
	else
	{
		initTexture();
		return false;
	}
}

void ChocolateChipObject::recycle()
{
	removeFromParent();
	stopAllActions();
}

void ChocolateChipObject::showCrushEffect()
{
	auto animationShow = PoolController::getInstance()->getChocolateChipCrushShow();
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	//animationShow->setScale(1.5);
	if (getParent() != nullptr) getParent()->addChild(animationShow, 500);
}

#pragma endregion

#pragma region DonutObject

DonutObject::DonutObject()
{
	canMatch = false;
	receiveNearbyAffect = true;
}

bool DonutObject::crush(bool showEffect)
{
	if (showEffect) showCrushEffect();
	//pCell->afterTileCrushProc();
	poolController->recycleCookieTile(this);
	return true;
}

void DonutObject::showCrushEffect()
{
	auto animationShow = PoolController::getInstance()->getDonutCrushShow();
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	//animationShow->setScale(1.5);
	if (getParent() != nullptr) getParent()->addChild(animationShow, 500);
}

void DonutObject::recycle()
{
	removeFromParent();
	stopAllActions();
}

#pragma endregion

