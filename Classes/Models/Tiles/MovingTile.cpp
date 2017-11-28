#include "MovingTile.h"
#include "Controllers/ActionController.h"
#include "General/Utils.h"
#include "Controllers/SpawnController.h"


MovingTile::MovingTile()
{
}


MovingTile::~MovingTile()
{
}

void MovingTile::showScaleBouncingAction() const
{
	CKAction ckAction;
	auto actionController = ActionController::getInstance();
	ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	ckAction.action = actionController->createScaleBouncingAction(ckAction.node);
	actionController->pushAction(ckAction, false);
}

void MovingTile::showDirectionalScaleAction(const AdjacentDirs dir) const
{
	CKAction ckAction;
	auto actionController = ActionController::getInstance();
	ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	ckAction.action = actionController->createDirectionalScaleAction(ckAction.node, dir);
	actionController->pushAction(ckAction, false);
}

bool MovingTile::isMovable() const
{
	return true;
}

void MovingTile::showSwapAction(GridPos& gridPos, const std::function<void()> callback)
{
	const auto targetPos = Utils::Grid2BoardPos(gridPos);
	CKAction ckAction;
	const auto actionController = ActionController::getInstance();
	ckAction.node = reinterpret_cast<Node*>(this);
	ckAction.action = actionController->createTileMoveAction(getPosition(), targetPos, callback);
	actionController->pushAction(ckAction, false);
}

void MovingTile::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);

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
		initTexture(textureName);
	}
}
