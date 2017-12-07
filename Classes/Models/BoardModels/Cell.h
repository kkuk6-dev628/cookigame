#pragma once
#include "General/TypeDefs.h"
#include "General/Utils.h"
#include "Models/Tiles/MovingTile.h"
#include "Layers/BoardLayer.h"

class CookieTile;

class Cell
{
public:
	Cell();
	~Cell();

	CookieTile* getSourceTile() const { return getTileAtLayer(LayerId::Match); }
	void setSourceTile(CookieTile* pTile);

	CookieTile* getTileAtLayer(LayerId layer) const;
	void setTileToLayer(CookieTile* pTile, LayerId layer);

	MovingTile* getMovingTile() const { return static_cast<MovingTile*>(getTileAtLayer(LayerId::Match)); }

	void setBoardLayer(BoardLayer* bl) { boardLayer = bl; }
	BoardLayer* getBoardLayer() { return boardLayer; }

	Cell* getFallCell() const;

	bool containsSpawner() const { return layers->objectForKey(LayerId::Spawner) != nullptr; }
	bool containsPortal() const { return layers->objectForKey(LayerId::Portal) != nullptr; }

	void setGridPos(const char col, const char row) { gridPos.Col = col; gridPos.Row = row; }
	void clear();
	void crushCell(bool showCrushEffect = true);

	cocos2d::Vec2 getBoardPos(){ return Utils::Grid2BoardPos(gridPos); }

	void spawnMatchTile();

	GridPos gridPos;
	char fallDirection = 1; // top -> down, if -1 bottom -> up 
	bool isFixed = false;
	bool isFillable = true;
	bool isPass = false;
	

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

	BoardLayer* boardLayer;

	__Array* spawnedTiles;

};

