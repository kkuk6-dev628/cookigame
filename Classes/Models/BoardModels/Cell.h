#pragma once
#include "General/TypeDefs.h"
#include "General/Utils.h"
#include "Models/Tiles/MovingTile.h"
#include "Layers/BoardLayer.h"
#include "Models/Tiles/FixTiles.h"
#include "Models/Tiles/SpawnerObject.h"

class CookieTile;

class Cell : public Ref
{
public:
	Cell();
	~Cell();

	CookieTile* getSourceTile() const { return pSourceTile; }
	SpawnerObject* getSpawner() const { return static_cast<SpawnerObject*>(getTileAtLayer(LayerId::Spawner)); }
	void setSourceTile(CookieTile* pTile);

	CookieTile* getReservedTile() const { return reservedTile; }
	void setReservedTile(CookieTile* tile);
	void countDownReserveCount();

	CookieTile* getTileAtLayer(LayerId layer) const;
	void setTileToLayer(CookieTile* pTile, LayerId layer);
	void removeTileAtLayer(LayerId layer);

	MovingTile* getMovingTile() const { return static_cast<MovingTile*>(pSourceTile); }

	void setBoardLayer(BoardLayer* bl) { boardLayer = bl; }
	BoardLayer* getBoardLayer() { return boardLayer; }

	//Cell* getFallCell(std::list<PortalInletObject*>* portalInData ) const;
	//Cell* getDirectFallCell(std::list<PortalInletObject*>* portalInData);
	//Cell* getInclinedFallCell(std::list<PortalInletObject*>* portalInData);
	Cell* getFallDownCell() { return inWater ? upCell : downCell; }
	Cell* getFallUpCell() { return inWater ? downCell : upCell; }

	bool canFill() const { return !isOutCell && !isFixed && isEmpty; }
	bool canFall() const { return !isOutCell && !isFixed && !isEmpty; }

	bool containsSpawner() const { return layers->find(LayerId::Spawner) != layers->end(); }
	bool containsDisplayCase() const;
	bool containsPortalOut() const;
	bool containsPortalIn() const;
	bool containsThoppler() const { return layers->find(LayerId::Toppling) != layers->end() || pSourceTile->getType() == TOPPLINGOBJECT || pSourceTile->getType() == HOPPLINGOBJECT; }
	bool containsPopsicle() const { return layers->find(LayerId::UnderCover) != layers->end(); }
	bool containsIceCover() const { return layers->find(LayerId::Cover) != layers->end(); }
	bool containsWaffle() const;
	bool isReceiveNearbyAffect();
	bool isNoShuffleCell();
	bool canMatch() const
	{
		return !isOutCell && !isEmpty && getMovingTile() != nullptr && getMovingTile()->canMatch;
	}
	bool canMove() const
	{
		return !isOutCell && !isEmpty && getMovingTile() != nullptr && !isFixed;
	}
	bool canMoveCream() const
	{
		return !isOutCell && !isFixed;
	}
	bool isRainbowCell() const
	{
		return !isOutCell && !isEmpty && getMovingTile() != nullptr && getMovingTile()->getMovingTileType() == +MovingTileTypes::RainbowObject;
	}

	Cell* findPortalInCell(std::list<PortalInletObject*>* portalInData) const;
	PortalOutletObject* getPortalOut() const;

	void setGridPos(const char col, const char row) { gridPos.Col = col; gridPos.Row = row; boardPos = Utils::Grid2BoardPos(gridPos); }
	void setNearbyCrushColor(TileColors color) const { if (pSourceTile != nullptr) pSourceTile->setNearbyColor(color); }
	void clear();
	void crushNearbyCells();
	void receiveNearbyAffect();
	void afterTileCrushProc();
	bool crushCell(bool showCrushEffect = true);
	bool crushUnderTiles(LayerId layerId);
	void createShuffleShow();
	
	cocos2d::Vec2 getBoardPos(){ return boardPos; }

	void spawnMatchTile();
	void spawnPathMover();
	MovingTile* getSpawnedTile() const;
	void resetSpawnedTile() { spawnedTile = nullptr; }

	void fillDisplayCase();
	GridPos gridPos;
	Vec2 boardPos;
	Direction fallDirection = Direction::S; // top -> down, if N bottom -> up 
	bool isOutCell = false;
	bool isFixed = false;
	bool isFillable = true;
	bool canPass = true;
	bool dirty = false;
	bool noShuffleCell = false;

	TileColors tileColor = TileColors::any;

	TileShowObject* shuffleShowObject = nullptr;
	Cell* shuffleResultCell = nullptr;

	char reserveCount = 0;

	bool hiderSearch = false;

public:
	Cell* upCell = nullptr;
	Cell* downCell = nullptr;
	Cell* leftCell = nullptr;
	Cell* rightCell = nullptr;
	bool isEmpty = true;
	bool inWater = false;

private:
	std::map<LayerId, CookieTile*>* layers;
	CookieTile* pSourceTile = nullptr;
	CookieTile* reservedTile = nullptr;
	MovingTile* spawnedTile = nullptr;

	BoardLayer* boardLayer;

	__Array* spawnedTiles;

};

