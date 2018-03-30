#include "SpawnController.h"
#include "General/Utils.h"
#include "Models/Tiles/SpawnerObject.h"

SpawnController* SpawnController::instance = nullptr;
SpawnController::SpawnController()
{
	colorTable = new std::map<TileColors, float>();
	pendingSpawnTypes = new std::list<std::string>;
}


SpawnController::~SpawnController()
{
	CC_SAFE_DELETE(colorTable);
}

SpawnController* SpawnController::getInstance()
{
	if (instance == nullptr)
		instance = new SpawnController();
	return instance;
}

void SpawnController::setColorTable(TileColorsTable& ct) const
{
	auto sum = 0.0f;
	for (const auto pair : *ct)
	{
		sum += pair.second;
	}
	auto cval = 0.0f;
	for (auto pair : *ct)
	{
		cval += pair.second / sum;
		colorTable->insert({ pair.first, cval });
	}

}

TileColors SpawnController::getSpawnColor() const
{
	const auto rnd = cocos2d::rand_0_1();
	for (auto pair : *colorTable)
	{
		if (rnd <= pair.second)
		{
			return pair.first;
		}
	}

	return TileColors::_from_integral(rnd * 6);
}

MovingTileTypes SpawnController::getSpawnType(std::string spawnerName, int spawnedCount, bool inWater) const
{
	std::string spawnTypeString = "LayeredMatchObject";

	if(inWater && liquidSpawnTable != nullptr)
	{
		for (auto table : *liquidSpawnTable)
		{
			if (Utils::checkSpawn(spawnedCount, table.Percent))
			{
				return MovingTileTypes::_from_string((*table.Type).c_str());
			}
		}
	}
	if(spawnerName == "normal")
	{
		if (pendingSpawnTypes->size() > 0)
		{
			spawnTypeString = pendingSpawnTypes->front();
			pendingSpawnTypes->pop_front();
			return MovingTileTypes::_from_string(spawnTypeString.c_str());
		}
		if (spawnTable != nullptr && spawnTable->size() > 0)
		{
			for (auto table : *spawnTable)
			{
				if (Utils::checkSpawn(spawnedTilesCount, table.Percent))
				{
					pendingSpawnTypes->push_back(*table.Type);
				}
			}

			if (pendingSpawnTypes->size() > 0)
			{
				spawnTypeString = pendingSpawnTypes->front();
				pendingSpawnTypes->pop_front();
				return MovingTileTypes::_from_string(spawnTypeString.c_str());
			}
		}
	}
	else if(customSpawnTable != nullptr)
	{
		if(customSpawnTable->find(spawnerName) != customSpawnTable->end())
		{
			auto spawnTable = customSpawnTable->at(spawnerName);
			auto spt = spawnTable.getSpawnTable();
			if(spt != nullptr)
			{
				for (auto table : *spt)
				{
					if (Utils::checkSpawn(spawnedCount, table.Percent))
					{
						return MovingTileTypes::_from_string((*table.Type).c_str());
					}
				}
			}

		}
	}
	return MovingTileTypes::LayeredMatchObject;
}

void SpawnController::clearSpawners()
{
	if(spawnersList != nullptr && spawnersList->size() > 0)
	{
		spawnersList->clear();
	}
}

void SpawnController::addSpawner(SpawnerObject* spawner)
{
	if(spawnersList == nullptr)
	{
		spawnersList = new std::list<SpawnerObject*>;
	}
	spawnersList->push_back(spawner);
}

void SpawnController::resetSpawners()
{
	for(auto spawner : *spawnersList)
	{
		spawner->initSpawnedCount();
	}
}
