#include "Cell.h"
#include "Models/Tiles/CookieTile.h"
#include "Models/Tiles/SpawnerObject.h"
#include "Controllers/PoolController.h"
#include "Models/Tiles/LayeredCrackerTile.h"
#include "Models/Tiles/DisplayCaseObject.h"
#include "Models/Tiles/ThopplerTile.h"

Cell::Cell()
{
	pSourceTile = nullptr;
	layers = new std::map<LayerId, CookieTile*>;
}


Cell::~Cell()
{
	CC_SAFE_DELETE(layers);
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
	if(layers->find(layer) == layers->end())
	{
		return nullptr;
	}
	const auto tile = layers->at(layer);
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
	if(pTile == nullptr)
	{
		return;
	}
	if (layer == +LayerId::Match)
	{
		//if(pSourceTile != nullptr && pSourceTile->getParent() != nullptr)
		//{
		//	PoolController::getInstance()->recycleCookieTile(pSourceTile);
		//}
		pSourceTile = pTile;
		isEmpty = false;
		if (!containsDisplayCase())
		{
			isFixed = !pTile->isMovable();
			canPass = !isFixed;
		}
		if(pTile->getType() == PRETZELOBJECT || pTile->getType() == EMPTYOBJECT)
		{
			canPass = true;
		}
		tileColor = pTile->getTileColor();
	}
	pTile->setCell(this);
	if (pTile->getType() == DISPLAYCASEOBJECT)
	{
		isFixed = true;
		canPass = false;
	}
	layers->insert(std::pair<LayerId, CookieTile*>(layer, pTile));
}

void Cell::removeTileAtLayer(LayerId layer)
{
	if(layers->find(layer) != layers->end())
	{
		layers->erase(layer);
	}
}

void Cell::clear()
{
	isEmpty = true;
	auto displayCaseTile = getTileAtLayer(LayerId::Toppling);
	if(displayCaseTile != nullptr && displayCaseTile->getType() == DISPLAYCASEOBJECT)
	{
		isFixed = true;
		canPass = false;
	}
	else
	{
		isFixed = false;
		canPass = true;
	}
	pSourceTile = nullptr;
	removeTileAtLayer(LayerId::Match);
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

	return noShuffleCell;
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

	auto noClearFlag = pSourceTile->noClear;
	auto ret = pSourceTile->crush(true);
	if (ret && !noClearFlag)
	{
		clear();
		dirty = false;
	}

	auto thopplerTile = getTileAtLayer(LayerId::Toppling);
	if (thopplerTile != nullptr && (thopplerTile->getType() == TOPPLINGOBJECT || thopplerTile->getType() == HOPPLINGOBJECT))
	{
		if (ret)
		{
			static_cast<ThopplerTile*>(thopplerTile)->moveDown();
			setSourceTile(thopplerTile);
			removeTileAtLayer(LayerId::Toppling);
		}
	}

	return ret;
}

bool Cell::crushUnderTiles(LayerId layerId)
{
	auto underTile = getTileAtLayer(layerId);
	if(underTile != nullptr)
	{
		switch(layerId)
		{
			case LayerId::Waffle:
			{
				auto waffleTile = static_cast<WaffleObject*>(underTile);
				if(waffleTile != nullptr && waffleTile->getType() == WAFFLEOBJECT)
				{
					if(waffleTile->crush(true))
					{
						if (layers->find(LayerId::Waffle) != layers->end())
						{
							layers->erase(LayerId::Waffle);
						}
					}
					return true;
				}
				else
				{
					return false;
				}
			}
			case LayerId::Cover:
			{
				auto iceCoverTile = static_cast<IceCoverObject*>(underTile);
				if(iceCoverTile != nullptr && iceCoverTile->getType() == ICECOVEROBJECT)
				{
					if(iceCoverTile->crush(true))
					{
						if (layers->find(LayerId::Cover) != layers->end())
						{
							layers->erase(LayerId::Cover);
						}
						return true;
					}
					return false;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return false;
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
	auto spawnerObject = static_cast<SpawnerObject*>(layers->at(LayerId::Spawner));
	if(spawnerObject != nullptr)
	{
		spawnedTile = spawnerObject->spawnMovingTile();
		if(spawnedTile->getParent() == nullptr)
		{
			boardLayer->addChild(spawnedTile);
		}
		if(!isFixed)
		{
			setSourceTile(spawnedTile);
		}
	}
}

void Cell::spawnPathMover()
{
	auto spawnerObject = static_cast<SpawnerObject*>(layers->at(LayerId::Spawner));
	if (spawnerObject != nullptr)
	{
		spawnedTile = spawnerObject->spawnPathMover();
		if (spawnedTile->getParent() == nullptr)
		{
			boardLayer->addChild(spawnedTile);
		}
		if (!isFixed)
		{
			setSourceTile(spawnedTile);
		}
	}
}

MovingTile* Cell::getSpawnedTile() const
{
	//return getMovingTile();
	if(isFixed)
	{
		if (spawnedTile != nullptr)
		{
			return spawnedTile;
		}
		return getMovingTile();
	}
	else
	{
		return getMovingTile();
	}
}

void Cell::fillDisplayCase()
{
	auto dispObject = static_cast<DisplayCaseObject*>(getTileAtLayer(LayerId::Toppling));
	if(dispObject != nullptr && dispObject->getType() == DISPLAYCASEOBJECT)
	{
		if(dispObject->refill())
		{
			removeTileAtLayer(LayerId::Toppling);
			isFixed = false;
			canPass = true;
			isEmpty = true;
		}
	}
}

Cell* Cell::findPortalInCell(std::list<PortalInletObject*>* portalInData) const
{
	auto portalOut = getPortalOut();
	if (portalOut == nullptr) return nullptr;

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
	if(layers->find(LayerId::Portal) == layers->end())
	{
		return nullptr;
	}
	return static_cast<PortalOutletObject*>(layers->at(LayerId::Portal));
}

bool Cell::containsDisplayCase() const
{
	auto dispCase = getTileAtLayer(LayerId::Toppling);
	if(dispCase != nullptr && dispCase->getType() == DISPLAYCASEOBJECT)
	{
		return true;
	}
	return false;
}

bool Cell::containsPortalOut() const
{
	if(layers->size() > 0 && layers->find(LayerId::Portal) != layers->end())
	{
		auto portal = static_cast<FixTiles*>(layers->at(LayerId::Portal));
		return strcmp(portal->getType().c_str(), PORTALOUTLETOBJECT) == 0;
	}
	return false;
}

bool Cell::containsPortalIn() const
{
	if (layers->size() > 0 && layers->find(LayerId::Portal) != layers->end())
	{
		auto portal = static_cast<FixTiles*>(layers->at(LayerId::Portal));
		return strcmp(portal->getType().c_str(), PORTALINLETOBJECT) == 0;
	}
	return false;
}

bool Cell::containsWaffle() const
{
	auto waffleTile = getTileAtLayer(LayerId::Waffle);
	if(waffleTile != nullptr && waffleTile->getType() == WAFFLEOBJECT)
	{
		return true;
	}
	return false;
}
