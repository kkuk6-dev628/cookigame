#pragma once
#include "Models/DataModels/CustomSpawnTableItem.h"

class SpawnController
{
public:
	~SpawnController();
	static SpawnController* getInstance();
	void setColorTable(TileColorsTable ct) const;
	TileColors getSpawnColor() const;
	MovingTileTypes getSpawnType() const;

private:
	TileColorsTable colorTable;
	SpawnController();
	static SpawnController* instance;
};

