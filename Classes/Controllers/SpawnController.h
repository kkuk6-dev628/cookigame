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
	void setSpawnOnCollectSystem(SpawnOnCollectSystem* spawnSys) { spawnOnCollectSystem = spawnSys; }

	TileColors getSpawnColor() const;
	SpawnTable* getCountSpawnTable() const;
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

	SpawnOnCollectSystem* spawnOnCollectSystem;
	int spawnedTilesCount = 0;
	std::list<SpawnerObject*>* spawnersList = nullptr;
	std::list<SpawnTable*>* pendingSpawnTypes = nullptr;
};

