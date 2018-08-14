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
	actionController = ActionController::getInstance();
	if(!Node::init())
	{
		return false;
	}
	return true;
}

void MovingTile::showScaleBouncingAction()
{
	//CKAction ckAction;
	//ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	//ckAction.action = actionController->createScaleBouncingAction(ckAction.node);
	//actionController->pushAction(ckAction, false);
	initMovingTile();

	auto showObj = poolController->getTileShowObject();
	showObj->setSpriteFrame(textureSprite->getSpriteFrame());
	showObj->setPosition(getPosition());
	showObj->setAnchorPoint(Vec2(0.5, 0.5));
	if (getParent() != nullptr && showObj->getParent() == nullptr)
	{
		getParent()->addChild(showObj);
	}
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(showObj);
	ckAction.action = actionController->createScaleBouncingAction([=] {
		this->setVisible(true);
		PoolController::getInstance()->recycleTileShowObject(showObj);
		BoardController::fallingTileCount--;
	}, ckAction.node);
	setVisible(false);
	isMoving = true;
	movingDuration = static_cast<Sequence*>(ckAction.action)->getDuration();
	actionController->pushAction(ckAction, true);
	BoardController::fallingTileCount++;
}

void MovingTile::resetTileScale()
{
	this->textureSprite->setScale(1.0f);
}

void MovingTile::showDirectionalScaleAction(const AdjacentDirs dir)
{
	if (this->textureSprite == nullptr) return;
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	ckAction.action = actionController->createDirectionalScaleAction(ckAction.node, dir);
	actionController->pushAction(ckAction, true);
}

void MovingTile::showFallAction(FallPath* path)
{
	if (this->textureSprite == nullptr) return;
	auto showObj = poolController->getTileShowObject();
	showObj->setSpriteFrame(textureSprite->getSpriteFrame());
	showObj->setPosition(getPosition());
	showObj->setAnchorPoint(Vec2(0.5, 0.5));

	if(getParent() != nullptr && showObj->getParent() == nullptr)
	{
		getParent()->addChild(showObj);
	}

	initMovingTile();

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

void MovingTile::showMoveAction(Cell* cell)
{
	initMovingTile();

	auto showObj = poolController->getTileShowObject();
	showObj->setSpriteFrame(textureSprite->getSpriteFrame());
	showObj->setPosition(getPosition());
	showObj->setAnchorPoint(Vec2(0.5, 0.5));
	if (getParent() != nullptr && showObj->getParent() == nullptr)
	{
		getParent()->addChild(showObj);
	}
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(showObj);
	ckAction.action = actionController->createTileMoveAction(getPosition(), cell->getBoardPos(), [=] {
		this->setVisible(true);
		this->isMoving = false;
		this->movingDuration = 0.f;
		this->setPosition(cell->getBoardPos());
		PoolController::getInstance()->recycleTileShowObject(showObj);
		BoardController::fallingTileCount--;
	}, ckAction.node);
	setVisible(false);
	isMoving = true;
	movingDuration = static_cast<Sequence*>(ckAction.action)->getDuration();
	actionController->pushAction(ckAction, true);
	BoardController::fallingTileCount++;
}

bool MovingTile::crush(bool showEffect)
{
	switch (modifierType)
	{
	case ModifierTypes::CageModifier:
	case ModifierTypes::HoneyModifier:
		if(modifierSprite != nullptr)
		{
			modifierSprite->setVisible(false);
		}
		modifierType = ModifierTypes::None;
		if(pCell != nullptr) pCell->isFixed = false;
		if (movingTileType != +MovingTileTypes::DonutObject && movingTileType != +MovingTileTypes::ChocolateChipObject)
		{
			canMatch = true;
		}

		if (movingTileType != +MovingTileTypes::DonutObject && movingTileType != +MovingTileTypes::ChocolateChipObject)
		{
			receiveNearbyAffect = false;
		}
		return false;
	case ModifierTypes::DigDownModifier:
		{
			auto digDownObj = pCell->getTileAtLayer(LayerId::Cover);
			if (digDownObj != nullptr && digDownObj->getType() == DIGDOWNOBJECT)
			{
				if(digDownObj->crush(true))
				{
					modifierType = ModifierTypes::None;
					if (pCell != nullptr) pCell->isFixed = false;
					if (movingTileType != +MovingTileTypes::DonutObject && movingTileType != +MovingTileTypes::ChocolateChipObject)
					{
						canMatch = true;
					}

					if (movingTileType != +MovingTileTypes::DonutObject && movingTileType != +MovingTileTypes::ChocolateChipObject)
					{
						receiveNearbyAffect = false;
					}
				}
				return false;
			}
			return true;
		}
		break;
	default:
		return CookieTile::crush(showEffect);
		break;
	}
}

bool MovingTile::isMovable()
{
	if (modifierType == +ModifierTypes::CageModifier || modifierType == +ModifierTypes::HoneyModifier || modifierType == +ModifierTypes::DigDownModifier)
	{
		return false;
	}
	return true;
}

void MovingTile::showSwapAction(GridPos& gridPos, const std::function<void()> callback)
{
	initMovingTile();

	auto showObj = poolController->getTileShowObject();
	showObj->setSpriteFrame(textureSprite->getSpriteFrame());
	showObj->setPosition(getPosition());
	showObj->setAnchorPoint(Vec2(0.5, 0.5));
	if (getParent() != nullptr && showObj->getParent() == nullptr)
	{
		getParent()->addChild(showObj);
	}
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(showObj);
	auto boardPos = Utils::Grid2BoardPos(gridPos);
	ckAction.action = actionController->createTileMoveAction(getPosition(), boardPos, [=] {
		this->setVisible(true);
		this->isMoving = false;
		this->movingDuration = 0.f;
		this->setPosition(boardPos);
		PoolController::getInstance()->recycleTileShowObject(showObj);
		callback();
		BoardController::fallingTileCount--;
	}, ckAction.node);
	setVisible(false);
	isMoving = true;
	movingDuration = static_cast<Sequence*>(ckAction.action)->getDuration();
	actionController->pushAction(ckAction, true);
	BoardController::fallingTileCount++;

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
		if (+TileColors::any == color || color == +TileColors::random)
		{
			color = SpawnController::getInstance()->getSpawnColor();
		}

		std::string textureName;
		switch (movingTileType)
		{
		case MovingTileTypes::ChocolateCheesecakeObject:
			canMatch = true;
			if(direction == +Direction::any)
			{
				direction = Direction::W;
			}
			textureName = StringUtils::format("%s_%s_%s.png", type.c_str(), color._to_string(), direction._to_string());
			break;
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
			textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
			break;
		}
		TileBase::initTexture(textureName);
		if(textureSprite != nullptr) textureSprite->setContentSize(Size(MovingTileSize, MovingTileSize));
		setModifierTexture();
	}

}

void MovingTile::initMovingTile()
{
	stopAllActionsByTag(HINT_ACTION);
	setScale(1.0f, 1.0f);
}

bool MovingTile::containsHoneyModifier()
{
	return modifierType == +ModifierTypes::HoneyModifier;
}

void MovingTile::setHoneyModifier()
{
	modifierType = ModifierTypes::HoneyModifier;
	if (pCell != nullptr)
	{
		pCell->isFixed = true;
		receiveNearbyAffect = true;
	}
	setModifierTexture();
}

void MovingTile::setModifierTexture()
{
	std::string spriteName;
	if(modifierType == +ModifierTypes::CageModifier || modifierType == +ModifierTypes::HoneyModifier)
	{
		spriteName = StringUtils::format("%s.png", modifierType._to_string());
	}

	if (modifierType == +ModifierTypes::HoneyModifier)
	{
		canMatch = false;
	}
	else if(modifierType == +ModifierTypes::DigDownModifier)
	{
		receiveNearbyAffect = true;
		canMatch = false;
	}
	//else if(modifierType == +ModifierTypes::DigDownModifier)
	//{
	//	auto digDownObj = pCell->getTileAtLayer(LayerId::Cover);
	//	layers = digDownObj->getLayers();
	//	spriteName = StringUtils::format("DigDownObject_%d.png", layers);
	//}
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteName);
	initModifierWithSprite(spriteFrame);
}

void MovingTile::initModifierWithSprite(SpriteFrame* sprite)
{
	if (sprite == nullptr)
	{
		return;
	}
	if (modifierSprite == nullptr)
	{
		modifierSprite = Sprite::create();
		addChild(modifierSprite);
	}
	modifierSprite->setSpriteFrame(sprite);
	modifierSprite->setContentSize(Size(CellSize, CellSize));
	modifierSprite->setAnchorPoint(Vec2(0.5, 0.5));
	modifierSprite->setPosition(CellSize / 2, CellSize / 2);
	modifierSprite->setVisible(true);
}

void MovingTile::showCrushEffect()
{
	auto animationShow = PoolController::getInstance()->getMatchCrushShow(color);
	if(animationShow != nullptr)
	{
		animationShow->setPosition(getPosition());
		animationShow->setAnchorPoint(Vec2(0.5f, 0.5f));
		//animationShow->setScale(1.5);
		if(getParent() != nullptr) getParent()->addChild(animationShow, 500);
	}
}
