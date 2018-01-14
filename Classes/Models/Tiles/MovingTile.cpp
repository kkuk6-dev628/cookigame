#include "MovingTile.h"
#include "Controllers/ActionController.h"
#include "General/Utils.h"
#include "Controllers/SpawnController.h"
#include "Models/BoardModels/FallPath.h"
#include "Controllers/PoolController.h"
#include "Controllers/BoardController.h"

int BoardController::fallingTileCount;

MovingTile::MovingTile()
{
	actionController = ActionController::getInstance();
	receiveNearbyAffect = false;
}


MovingTile::~MovingTile()
{
}

bool MovingTile::init()
{
	if(!Node::init())
	{
		return false;
	}
	actionController = ActionController::getInstance();
	return true;
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
	if (this->textureSprite == nullptr) return;
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
			this->isMoving = false;
			this->movingDuration = 0.f;
			this->setPosition(Utils::Grid2BoardPos(this->gridPos));
			PoolController::getInstance()->recycleTileShowObject(showObj);
			BoardController::fallingTileCount--;
		}, 
		ckAction.node);
	setVisible(false);
	isMoving = true;
	movingDuration = static_cast<Sequence*>(ckAction.action)->getDuration();
	actionController->pushAction(ckAction, true);
	BoardController::fallingTileCount++;
}

bool MovingTile::crush(bool showEffect)
{
	return CookieTile::crush(showEffect);
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
		case MovingTileTypes::ChocolateChipObject:
			textureName = StringUtils::format("%s_%d.png", type.c_str(), layers);
			break;
		case MovingTileTypes::RainbowObject:
			canMatch = false;
			textureName = StringUtils::format("%s.png", type.c_str());
			break;
		case MovingTileTypes::DonutObject:
			canMatch = false;
			textureName = StringUtils::format("%s.png", type.c_str());
			break;
		default:
			canMatch = true;
			if (+TileColors::any == color || color == +TileColors::random)
			{
				color = SpawnController::getInstance()->getSpawnColor();
			}
			textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
			break;
		}
		TileBase::initTexture(textureName);
		textureSprite->setContentSize(Size(MovingTileSize, MovingTileSize));
		//textureSprite->setScale(1.1);
	}

}

void MovingTile::showCrushEffect()
{
	auto animationShow = PoolController::getInstance()->getMatchCrushShow(color);
	animationShow->setPosition(getPosition());
	animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
	//animationShow->setScale(1.5);
	if(getParent() != nullptr) getParent()->addChild(animationShow, 500);
}
