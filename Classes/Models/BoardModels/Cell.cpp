#include "Cell.h"
#include "Models/Tiles/CookieTile.h"
#include "Models/Tiles/SpawnerObject.h"
#include "Controllers/PoolController.h"

Cell::Cell()
{
	pSourceTile = nullptr;
	layers = __Dictionary::create();
	layers->retain();
}


Cell::~Cell()
{
	layers->release();
}

void Cell::setSourceTile(CookieTile* pTile)
{
	pSourceTile = pTile;
	pTile->setCell(this);
	layers->setObject(pTile, LayerId::Match);
	isEmpty = false;
}

CookieTile* Cell::getTileAtLayer(LayerId layer) const
{
	const auto tile = static_cast<CookieTile*>(layers->objectForKey(layer._to_integral()));
	return tile;
}

void Cell::setTileToLayer(CookieTile* pTile, const LayerId layer)
{
	if (layer._to_integral() == LayerId::Match)
	{
		isEmpty = false;
	}
	pTile->setCell(this);
	layers->setObject(pTile, layer._to_integral());
}

void Cell::clear()
{
	isEmpty = true;
	pSourceTile = nullptr;
}

void Cell::crushCell(bool showCrushEffect)
{
	if(showCrushEffect)
	{
		getSourceTile()->showCrushEffect();
	}
	PoolController::getInstance()->recycleCookieTile(getSourceTile());
	clear();
}

void Cell::spawnMatchTile()
{
	auto spawnerObject = static_cast<SpawnerObject*>(layers->objectForKey(LayerId::Spawner));
	if(spawnerObject != nullptr)
	{
		auto spawnedTile = spawnerObject->spawnMovingTile();
		setSourceTile(spawnedTile);
		boardLayer->addChild(spawnedTile);
	}
}

Cell* Cell::getFallCell() const
{
	if (fallDirection > 0)
	{
		if(upCell == nullptr)
		{
			return nullptr;
		}

		if (!upCell->isFixed)
		{
			return upCell;
		}
		else
		{
			if(upCell->leftCell != nullptr && !upCell->leftCell->isFixed)
			{
				return upCell->leftCell;
			}
			else if(upCell->rightCell != nullptr && !upCell->rightCell->isFixed)
			{
				return upCell->rightCell;
			}
			else
			{
				return nullptr;
			}
		}
	}
	return nullptr;
}
