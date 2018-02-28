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

	bool containsSpawner() const { return layers->objectForKey(LayerId::Spawner) != nullptr; }
	bool containsPortalOut() const;
	bool containsPortalIn() const;
	bool containsThoppler() const { return layers->objectForKey(LayerId::Toppling) != nullptr; }
	bool containsPopsicle() const { return layers->objectForKey(LayerId::UnderCover) != nullptr; }
	bool containsIceCover() const { return layers->objectForKey(LayerId::Cover) != nullptr; }
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
	void clear();
	void crushNearbyCells();
	void receiveNearbyAffect();
	void afterTileCrushProc();
	bool crushCell(bool showCrushEffect = true);
	bool crushUnderTiles(LayerId layerId);
	void createShuffleShow();
	
	cocos2d::Vec2 getBoardPos(){ return boardPos; }

	void spawnMatchTile();

	GridPos gridPos;
	Vec2 boardPos;
	Direction fallDirection = Direction::S; // top -> down, if N bottom -> up 
	bool isOutCell = false;
	bool isFixed = false;
	bool isFillable = true;
	bool isPass = false;
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
	__Dictionary* layers;
	CookieTile* pSourceTile = nullptr;
	CookieTile* reservedTile = nullptr;

	BoardLayer* boardLayer;

	__Array* spawnedTiles;

};

