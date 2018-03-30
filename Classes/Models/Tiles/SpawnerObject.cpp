#include "SpawnerObject.h"
#include "Controllers/PoolController.h"
#include "Models/BoardModels/Cell.h"
#include "General/Constants.h"

SpawnerObject::SpawnerObject()
{
	spawnController = SpawnController::getInstance();
	poolController = PoolController::getInstance();
}


SpawnerObject::~SpawnerObject()
{
}

MovingTile* SpawnerObject::spawnMovingTile()
{
	spawnController->countSpawnedTiles();
	auto spawnColor = spawnController->getSpawnColor();
	auto spawnType = spawnController->getSpawnType(name, totalSpawnedCount, pCell->inWater);
	if(spawnType == +MovingTileTypes::DonutObject)
	{
		cocos2d::log("Spawn DonutObject");
	}
	auto spawnTile = static_cast<MovingTile*>(poolController->getCookieTile(spawnType._to_string()));

	spawnTile->setTileColor(spawnColor);
	auto spawnedPos = Utils::Grid2BoardPos(gridPos);

	auto dir = direction == +Direction::S ? 1 : -1;
	spawnedPos.add(Vec2(0, (spawnedCount + 1) * dir * CellSize));
	spawnedCount++;
	spawnTile->initWithGrid(gridPos.Col, gridPos.Row);
	spawnTile->setPosition(spawnedPos);
	spawnTile->initWithType(spawnType._to_string(), spawnColor);
	totalSpawnedCount++;
	//spawnTile->setContentSize(Size(CellSize, CellSize));
	return spawnTile;
}

MovingTile* SpawnerObject::spawnPathMover()
{
	spawnController->countSpawnedTiles();
	auto spawnColor = spawnController->getSpawnColor();
	MovingTileTypes spawnType = MovingTileTypes::PathMoverMatchObject;
	auto spawnTile = static_cast<MovingTile*>(poolController->getCookieTile(spawnType._to_string()));

	spawnTile->setTileColor(spawnColor);
	auto spawnedPos = Utils::Grid2BoardPos(gridPos);

	auto dir = direction == +Direction::S ? 1 : -1;
	spawnedPos.add(Vec2(0, (spawnedCount + 1) * dir * CellSize));
	spawnedCount++;
	spawnTile->initWithGrid(gridPos.Col, gridPos.Row);
	spawnTile->setPosition(spawnedPos);
	spawnTile->initWithType(spawnType._to_string(), spawnColor);
	totalSpawnedCount++;
	//spawnTile->setContentSize(Size(CellSize, CellSize));
	return spawnTile;
}

char SpawnerObject::fallDirection() const
{
	return getCell()->fallDirection;
}


