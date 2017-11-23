#pragma once
#include "General/TypeDefs.h"
#include "Tile.h"

class CookieTile : public TileBase
{
public:
	CookieTile();
	~CookieTile();
	CREATE_FUNC(CookieTile);

	Cell* getCell() const { return pCell; }
	void setCell(Cell* cell);
	void initWithJson(rapidjson::Value& json);

protected:
	Cell* pCell;
	GridPos gridPos;

	std::string type;
	char layers;
	TileColors color = TileColors::any;
	std::string direction;
	std::string name;
	char priority;
	bool createSpread;
	bool mirror;
	ModifierTypes modifierType = ModifierTypes::NoShuffleModifier;
	GridSizeT gridSize;
	SetTypes setType = SetTypes::set;
	bool createJelly;
	HiderSegmentPosition hiderSegPosition = HiderSegmentPosition::body;
	std::string chain;
	std::string spawnType;
	SpawnTable spawnData;
	char zone;
	std::string spawnTableName;
	TileColors slot1 = TileColors::any;
	TileColors slot2 = TileColors::any;
	TileColors slot3 = TileColors::any;
	TileColors slot4 = TileColors::any;
	TileColors slot5 = TileColors::any;
	TileColors slot6 = TileColors::any;
	char sequenceId;
	char power;

};

class LayeredMatchObject : public CookieTile
{

};

class WaffleObject : public CookieTile
{

};

