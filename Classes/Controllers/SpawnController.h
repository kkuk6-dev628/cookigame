#pragma once
#include "Models/DataModels/CustomSpawnTableItem.h"

class SpawnerObject;

class SpawnController
{
public:
	~SpawnController();
	static SpawnController* getInstance();
	void setColorTable(TileColorsTable& ct) const;
	void setSpawnTable(SpawnTablesList ct) { spawnTable = ct; }
	void setLiquidSpawnTable(SpawnTablesList ct) { liquidSpawnTable = ct; }
	void setCustomSpawnTable(std::map<std::string, CustomSpawnTableItem>* ct) { customSpawnTable = ct; }
	TileColors getSpawnColor() const;
	SpawnTable* getSpawnTable(std::string spawnerName, int spawnedCount, bool inWater = false) const;
	void countSpawnedTiles() { spawnedTilesCount++; }

	void clearSpawners();
	void addSpawner(SpawnerObject* spawner);
	void resetSpawners();

private:
	TileColorsTable colorTable;
	SpawnController();
	static SpawnController* instance;
	SpawnTablesList spawnTable;
	SpawnTablesList liquidSpawnTable;
	std::map<std::string, CustomSpawnTableItem>* customSpawnTable = nullptr;

	int spawnedTilesCount = 0;
	std::list<SpawnerObject*>* spawnersList = nullptr;
	std::list<SpawnTable*>* pendingSpawnTypes = nullptr;
};

