#include "SpawnController.h"
#include "General/Utils.h"
#include "Models/Tiles/SpawnerObject.h"

SpawnController* SpawnController::instance = nullptr;
SpawnController::SpawnController()
{
	colorTable = new std::map<TileColors, float>();
	pendingSpawnTypes = new std::list<SpawnTable*>;
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

SpawnTable* SpawnController::getCountSpawnTable() const
{
	if(spawnTable == nullptr)
	{
		return nullptr;
	}
	for (auto table = spawnTable->begin(); table != spawnTable->end(); table++)
	{
		if (table->IntMax > 0 || table->IntMin > 0)
		{
			return &*table;
		}
	}
	return nullptr;
}

SpawnTable* SpawnController::getSpawnTable(std::string spawnerName, int spawnedCount, bool inWater) const
{
	SpawnTable* spawnTypeString = nullptr;

	if(spawnerName == "normal" || spawnerName == "")
	{
		if (inWater && liquidSpawnTable != nullptr)
		{
			auto percent = 0.0f;
			for (auto table = liquidSpawnTable->begin(); table != liquidSpawnTable->end(); table++)
			{
				percent += table->Percent;
				if (Utils::checkSpawn(spawnedCount, percent))
				{
					return &(*table);
				}
			}
		}
		if (pendingSpawnTypes->size() > 0)
		{
			spawnTypeString = pendingSpawnTypes->front();
			pendingSpawnTypes->pop_front();
			return spawnTypeString;
		}
		if (spawnTable != nullptr && spawnTable->size() > 0)
		{
			for (auto table = spawnTable->begin(); table != spawnTable->end(); table++)
			{
				if (Utils::checkSpawn(spawnedTilesCount, table->Percent))
				{
					pendingSpawnTypes->push_back(&*table);
				}
			}

			if (pendingSpawnTypes->size() > 0)
			{
				spawnTypeString = pendingSpawnTypes->front();
				pendingSpawnTypes->pop_front();
				return spawnTypeString;
			}
		}
	}
	else
	{
		if (forcedSpawnQueue != nullptr)
		{
			if (forcedSpawnQueue->find(spawnerName) != forcedSpawnQueue->end())
			{
				auto spawnTable = forcedSpawnQueue->at(spawnerName);
				auto spt = spawnTable.getSpawnTable();
				if (spt != nullptr && spt->size() > 0)
				{
					auto ret = new SpawnTable(spt->front());
					spt->pop_front();
					return ret;
				}

			}
		}
		if (customSpawnTable != nullptr)
		{
			if (customSpawnTable->find(spawnerName) != customSpawnTable->end())
			{
				auto spawnTable = customSpawnTable->at(spawnerName);
				auto spt = spawnTable.getSpawnTable();
				if (spt != nullptr)
				{
					for (auto table = spt->begin(); table != spt->end(); table++)
					{
						if (Utils::checkSpawn(spawnedCount, table->Percent))
						{
							return &*table;
						}
					}
				}

			}
		}
	}
	return nullptr;
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
	if (spawnersList == nullptr) return;

	for(auto spawner : *spawnersList)
	{
		spawner->initSpawnedCount();
	}
}
