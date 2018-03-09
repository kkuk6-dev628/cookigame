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
	auto spawnType = spawnController->getSpawnType(name);
	auto spawnTile = static_cast<MovingTile*>(poolController->getCookieTile(spawnType._to_string()));

	spawnTile->setTileColor(spawnColor);
	auto spawnedPos = Utils::Grid2BoardPos(gridPos);

	auto dir = direction == +Direction::S ? 1 : -1;
	spawnedPos.add(Vec2(0, (spawnedCount + 1) * dir * CellSize));
	spawnedCount++;
	spawnTile->initWithGrid(gridPos.Col, gridPos.Row);
	spawnTile->setPosition(spawnedPos);
	spawnTile->initWithType(spawnType._to_string(), spawnColor);
	//spawnTile->setContentSize(Size(CellSize, CellSize));
	return spawnTile;
}

char SpawnerObject::fallDirection() const
{
	return getCell()->fallDirection;
}


