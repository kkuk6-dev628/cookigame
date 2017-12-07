#include "MovingTile.h"
#include "Controllers/ActionController.h"
#include "General/Utils.h"
#include "Controllers/SpawnController.h"
#include "Models/BoardModels/FallPath.h"
#include "Controllers/PoolController.h"


MovingTile::MovingTile()
{
	actionController = ActionController::getInstance();
}


MovingTile::~MovingTile()
{
}

void MovingTile::showScaleBouncingAction() const
{
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	ckAction.action = actionController->createScaleBouncingAction(ckAction.node);
	actionController->pushAction(ckAction, false);
}

void MovingTile::showDirectionalScaleAction(const AdjacentDirs dir) const
{
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	ckAction.action = actionController->createDirectionalScaleAction(ckAction.node, dir);
	actionController->pushAction(ckAction, false);
}

void MovingTile::showFallAction(FallPath* path)
{
	auto showObj = poolController->getTileShowObject();
	showObj->setSpriteFrame(textureSprite->getSpriteFrame());
	showObj->setPosition(getPosition());
	showObj->setAnchorPoint(Vec2(0.5, 0.5));

	getParent()->addChild(showObj);

	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(showObj);
	ckAction.action = actionController->createMoveThroughAction(path, [this, showObj]()
		{
			this->setVisible(true);
			PoolController::getInstance()->recycleTileShowObject(showObj);
		}, 
		ckAction.node);
	setVisible(false);

	actionController->pushAction(ckAction, true);
}

bool MovingTile::isMovable() const
{
	return true;
}

void MovingTile::showSwapAction(GridPos& gridPos, const std::function<void()> callback)
{
	const auto targetPos = Utils::Grid2BoardPos(gridPos);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(this);
	ckAction.action = actionController->createTileMoveAction(getPosition(), targetPos, callback, ckAction.node);
	actionController->pushAction(ckAction, false);
}

void MovingTile::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	initTexture();
}

void MovingTile::initTexture()
{
	if (MovingTileTypes::_is_valid_nocase(type.c_str()))
	{
		movingTileType = MovingTileTypes::_from_string_nocase(type.c_str());

		std::string textureName;
		switch (movingTileType)
		{
		case MovingTileTypes::RainbowObject:
		case MovingTileTypes::DonutObject:
			textureName = type;
			break;
		default:
			if (TileColors::any == color._to_integral() || color._to_integral() == TileColors::random)
			{
				color = SpawnController::getInstance()->getSpawnColor();
			}
			textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
			break;
		}
		TileBase::initTexture(textureName);
	}

}

void MovingTile::showCrushEffect()
{
	auto animationShow = PoolController::getInstance()->getMatchCrushShow(color);
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	animationShow->setScale(1.5);
	getParent()->addChild(animationShow, 500);
}
