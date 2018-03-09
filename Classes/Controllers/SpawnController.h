#pragma once
#include "Models/DataModels/CustomSpawnTableItem.h"

class SpawnController
{
public:
	~SpawnController();
	static SpawnController* getInstance();
	void setColorTable(TileColorsTable& ct) const;
	void setSpawnTable(SpawnTablesList ct) { spawnTable = ct; }
	TileColors getSpawnColor() const;
	MovingTileTypes getSpawnType(std::string spawnerName) const;
	void countSpawnedTiles() { spawnedTilesCount++; }

private:
	TileColorsTable colorTable;
	SpawnController();
	static SpawnController* instance;
	SpawnTablesList spawnTable;

	int spawnedTilesCount = 0;

	std::list<std::string>* pendingSpawnTypes = nullptr;
};

