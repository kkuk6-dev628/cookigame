#include "SpawnController.h"

SpawnController* SpawnController::instance = nullptr;
SpawnController::SpawnController()
{
	colorTable = new std::map<TileColors, float>();
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

void SpawnController::setColorTable(TileColorsTable ct) const
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

MovingTileTypes SpawnController::getSpawnType() const
{
	return MovingTileTypes::LayeredMatchObject;
}
