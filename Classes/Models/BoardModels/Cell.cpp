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
	setTileToLayer(pTile, LayerId::Match);
	if(pTile->getParent() == nullptr)
	{
		boardLayer->addChild(pTile);
	}
}

CookieTile* Cell::getTileAtLayer(LayerId layer) const
{
	const auto tile = static_cast<CookieTile*>(layers->objectForKey(layer));
	return tile;
}

void Cell::setReservedTile(CookieTile* tile)
{
	if(reservedTile != nullptr)
	{
		PoolController::getInstance()->recycleCookieTile(reservedTile);
	}
	reservedTile = tile; 
	reservedTile->initWithGrid(gridPos.Col, gridPos.Row);
	reserveCount = 1;
}

void Cell::countDownReserveCount()
{
	if (reserveCount > 0)
	{
		reserveCount--;
	}
}

void Cell::setTileToLayer(CookieTile* pTile, const LayerId layer)
{
	if (layer == +LayerId::Match)
	{
		pSourceTile = pTile;
		isEmpty = false;
		if(pTile->getMovingTileType() == +MovingTileTypes::FixTile)
		{
			isFixed = true;
		}
	}
	pTile->setCell(this);
	layers->setObject(pTile, layer._to_integral());
}

void Cell::clear()
{
	isEmpty = true;
	isFixed = false;
	pSourceTile = nullptr;
	layers->removeObjectForKey(LayerId::Match);
}

void Cell::crushNearbyCells()
{
	if (upCell != nullptr) upCell->receiveNearbyAffect();
	if (downCell != nullptr) downCell->receiveNearbyAffect();
	if (leftCell != nullptr) leftCell->receiveNearbyAffect();
	if (rightCell != nullptr) rightCell->receiveNearbyAffect();
}

void Cell::receiveNearbyAffect()
{
	if(getSourceTile() != nullptr && getSourceTile()->receiveNearbyAffect)
	{
		crushCell(true);
	}
}

void Cell::afterTileCrushProc()
{
	if(getSourceTile() != nullptr && getSourceTile()->canMatch) crushNearbyCells();
	clear();
	dirty = false;
}

void Cell::crushCell(bool showCrushEffect)
{
	if(isOutCell || isEmpty || getSourceTile() == nullptr)
	{
		return;
	}
	if(getSourceTile() != nullptr && getSourceTile()->crush(showCrushEffect))
	{
		//afterTileCrushProc();
	}
}

//SpawnerObject* Cell::getSpawnerObject()
//{
//	
//}

void Cell::spawnMatchTile()
{
	auto spawnerObject = static_cast<SpawnerObject*>(layers->objectForKey(LayerId::Spawner));
	if(spawnerObject != nullptr)
	{
		auto spawnedTile = spawnerObject->spawnMovingTile();
		setSourceTile(spawnedTile);
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

		if (!upCell->isFixed && !upCell->isOutCell)
		{
			return upCell;
		}
		else
		{
			if(upCell->leftCell != nullptr && !upCell->leftCell->isFixed && !upCell->leftCell->isOutCell)
			{
				return upCell->leftCell;
			}
			else if(upCell->rightCell != nullptr && !upCell->rightCell->isFixed && !upCell->rightCell->isOutCell)
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
