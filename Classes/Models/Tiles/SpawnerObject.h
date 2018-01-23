#pragma once
#include "CookieTile.h"
#include "MovingTile.h"
#include "Controllers/SpawnController.h"


class SpawnerObject : public CookieTile
{
public:
	SpawnerObject();
	~SpawnerObject();

	CREATE_FUNC(SpawnerObject)

	MovingTile* spawnMovingTile();
	void initSpawnedCount() { spawnedCount = 0; }
	void initSpawner() { direction = Direction::S; }

private:
	SpawnController* spawnController;
	PoolController* poolController;

	char spawnedCount = 0;

	char fallDirection() const;

protected:

};

