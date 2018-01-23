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
		isFixed = !pTile->isMovable();
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

bool Cell::isReceiveNearbyAffect()
{
	if(isEmpty || isOutCell || getSourceTile() == nullptr)
	{
		return false;
	}
	return getSourceTile()->receiveNearbyAffect;
}

bool Cell::isNoShuffleCell()
{
	if(isOutCell || isEmpty || isFixed)
	{
		return true;
	}
	if(getMovingTile()->getModifierType() != +ModifierTypes::None)
	{
		return true;
	}
	return false;
}

void Cell::afterTileCrushProc()
{
	if(getSourceTile() != nullptr && getSourceTile()->canMatch) crushNearbyCells();
	clear();
	dirty = false;
}

bool Cell::crushCell(bool showCrushEffect)
{
	if(isOutCell || isEmpty || getSourceTile() == nullptr)
	{
		return false;
	}

	auto targetObj = getTileAtLayer(LayerId::Target);
	if(targetObj != nullptr && targetObj->getLayers() > 0)
	{
		targetObj->setLayers(targetObj->getLayers() - 1);
	}

	auto ret = getSourceTile()->crush(true);
	if (ret) 
	{
		clear();
		dirty = false;
	}
	return ret;
}

void Cell::createShuffleShow()
{
	if(shuffleShowObject == nullptr)
	{
		shuffleShowObject = PoolController::getInstance()->getTileShowObject();
	}
	shuffleShowObject->setSpriteFrame(getMovingTile()->getMainSpriteFrame());
	shuffleShowObject->setPosition(getBoardPos());
	shuffleShowObject->setAnchorPoint(Vec2(0.5, 0.5));
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

Cell* Cell::getFallCell(std::list<PortalInletObject*>* portalInData) const
{
	if (!inWater)
	{
		if(containsPortalOut())
		{
			return findPortalInCell(portalInData);
		}
		if(upCell == nullptr)
		{
			return nullptr;
		}

		if (!upCell->isFixed && !upCell->isOutCell && !upCell->inWater)
		{
			return upCell;
		}
		else
		{
			if(upCell->leftCell != nullptr && !upCell->leftCell->isFixed && !upCell->leftCell->isOutCell && !upCell->leftCell->inWater)
			{
				return upCell->leftCell;
			}
			else if(upCell->rightCell != nullptr && !upCell->rightCell->isFixed && !upCell->rightCell->isOutCell && !upCell->rightCell->inWater)
			{
				return upCell->rightCell;
			}
			else if (leftCell != nullptr && !leftCell->isFixed && !leftCell->isOutCell && !leftCell->isEmpty && !leftCell->inWater && downCell != nullptr && !downCell->isOutCell)
			{
				return leftCell;
			}
			else if (rightCell != nullptr && !rightCell->isFixed && !rightCell->isOutCell && !rightCell->isEmpty && !rightCell->inWater && downCell != nullptr && !downCell->isOutCell)
			{
				return rightCell;
			}
			{
				return nullptr;
			}
		}
	}
	else
	{
		//if (containsPortalOut())
		//{
		//	return findPortalInCell(portalInData);
		//}
		if (downCell == nullptr)
		{
			return nullptr;
		}

		if (!downCell->isFixed && !downCell->isOutCell && downCell->inWater)
		{
			return downCell;
		}
		else
		{
			if (downCell->leftCell != nullptr && !downCell->leftCell->isFixed && !downCell->leftCell->isOutCell && downCell->leftCell->inWater)
			{
				return downCell->leftCell;
			}
			else if (downCell->rightCell != nullptr && !downCell->rightCell->isFixed && !downCell->rightCell->isOutCell && downCell->rightCell->inWater)
			{
				return downCell->rightCell;
			}
			else if (leftCell != nullptr && !leftCell->isFixed && !leftCell->isOutCell && !leftCell->isEmpty && leftCell->inWater && upCell != nullptr && !upCell->isOutCell)
			{
				return leftCell;
			}
			else if (rightCell != nullptr && !rightCell->isFixed && !rightCell->isOutCell && !rightCell->isEmpty && rightCell->inWater && upCell != nullptr && !upCell->isOutCell)
			{
				return rightCell;
			}
			{
				return nullptr;
			}
		}
	}
	return nullptr;
}

Cell* Cell::findPortalInCell(std::list<PortalInletObject*>* portalInData) const
{
	auto portalOut = getPortalOut();
	for(auto portal : *portalInData)
	{
		if(portal->getTileColor() == portalOut->getTileColor())
		{
			return portal->getCell();
		}
	}
	return nullptr;
}

PortalOutletObject* Cell::getPortalOut() const
{
	return static_cast<PortalOutletObject*>(layers->objectForKey(LayerId::Portal));
}

bool Cell::containsPortalOut() const
{
	if(layers->objectForKey(LayerId::Portal) != nullptr)
	{
		auto portal = static_cast<FixTiles*>(layers->objectForKey(LayerId::Portal));
		return strcmp(portal->getType().c_str(), "PortalOutletObject") == 0;
	}
	return false;
}
