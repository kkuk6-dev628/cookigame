#pragma once
#include "General/TypeDefs.h"
#include "Tile.h"

class CookieTile : public TileBase
{
public:
	CookieTile();
	//~CookieTile();
	CREATE_FUNC(CookieTile)

	Cell* getCell() const { return pCell; }
	void setCell(Cell* cell);

	MovingTileTypes getMovingTileType() const;
	void setMovingTileType(MovingTileTypes tileType);

	void setCellPos();
	virtual void initWithJson(rapidjson::Value& json);
	virtual void initWithType(std::string typeName, TileColors color = TileColors::any);
	virtual void showCrushEffect() {};
	virtual void initTexture() {};

	virtual bool crush(bool showEffect);

	bool canMatch = true;
	int matchId = 0;
	bool receiveNearbyAffect = false;

protected:
	Cell* pCell;

	std::string type;
	char layers = 0;
	TileColors color = TileColors::any;
	std::string direction;
	std::string name;
	char priority = 0;
	bool createSpread = false;
	bool mirror = false;
	ModifierTypes modifierType = ModifierTypes::NoShuffleModifier;
	GridSizeT gridSize;
	SetTypes tileSetType = SetTypes::set;
	bool createJelly = false;
	HiderSegmentPosition hiderSegPosition = HiderSegmentPosition::body;
	std::string chain;
	std::string spawnType;
	SpawnTable spawnData;
	char zone = 0;
	std::string spawnTableName;
	TileColors slot1 = TileColors::any;
	TileColors slot2 = TileColors::any;
	TileColors slot3 = TileColors::any;
	TileColors slot4 = TileColors::any;
	TileColors slot5 = TileColors::any;
	TileColors slot6 = TileColors::any;
	char sequenceId = 0;
	char power = 0;

public:
	int getMatchId() const
	{
		return matchId;
	}

	void setMatchId(int match_id)
	{
		matchId = match_id;
	}

	std::string getType() const
	{
		return type;
	}

	void setType(const std::string& type)
	{
		this->type = type;
	}

	char getLayers() const
	{
		return layers;
	}

	void setLayers(char layers)
	{
		this->layers = layers;
	}

	TileColors getTileColor() const
	{
		return color;
	}

	void setTileColor(const TileColors& color)
	{
		this->color = color;
	}

	std::string getDirection() const
	{
		return direction;
	}

	void setDirection(const std::string& direction)
	{
		this->direction = direction;
	}

	std::string getTileName() const
	{
		return name;
	}

	void setTileName(const std::string& name)
	{
		this->name = name;
	}

	char getPriority() const
	{
		return priority;
	}

	void setPriority(char priority)
	{
		this->priority = priority;
	}

	bool getCreateSpread() const
	{
		return createSpread;
	}

	void setCreateSpread(bool create_spread)
	{
		createSpread = create_spread;
	}

	bool getMirror() const
	{
		return mirror;
	}

	void setMirror(bool mirror)
	{
		this->mirror = mirror;
	}

	ModifierTypes getModifierType() const
	{
		return modifierType;
	}

	void setModifierType(const ModifierTypes& modifier_type)
	{
		modifierType = modifier_type;
	}

	GridSizeT getGridSize() const
	{
		return gridSize;
	}

	void setGridSize(const GridSizeT& grid_size)
	{
		gridSize = grid_size;
	}

	bool getCreateJelly() const
	{
		return createJelly;
	}

	void setCreateJelly(bool create_jelly)
	{
		createJelly = create_jelly;
	}

	HiderSegmentPosition getHiderSegPosition() const
	{
		return hiderSegPosition;
	}

	void setHiderSegPosition(const HiderSegmentPosition& hider_seg_position)
	{
		hiderSegPosition = hider_seg_position;
	}

	std::string getChain() const
	{
		return chain;
	}

	void setChain(const std::string& chain)
	{
		this->chain = chain;
	}

	std::string getSpawnType() const
	{
		return spawnType;
	}

	void setSpawnType(const std::string& spawn_type)
	{
		spawnType = spawn_type;
	}

	SpawnTable getSpawnData() const
	{
		return spawnData;
	}

	void setSpawnData(const SpawnTable& spawn_data)
	{
		spawnData = spawn_data;
	}

	char getZone() const
	{
		return zone;
	}

	void setZone(char zone)
	{
		this->zone = zone;
	}

	std::string getSpawnTableName() const
	{
		return spawnTableName;
	}

	void setSpawnTableName(const std::string& spawn_table_name)
	{
		spawnTableName = spawn_table_name;
	}

	TileColors getSlot1() const
	{
		return slot1;
	}

	void setSlot1(const TileColors& slot1)
	{
		this->slot1 = slot1;
	}

	TileColors getSlot2() const
	{
		return slot2;
	}

	void setSlot2(const TileColors& slot2)
	{
		this->slot2 = slot2;
	}

	TileColors getSlot3() const
	{
		return slot3;
	}

	void setSlot3(const TileColors& slot3)
	{
		this->slot3 = slot3;
	}

	TileColors getSlot4() const
	{
		return slot4;
	}

	void setSlot4(const TileColors& slot4)
	{
		this->slot4 = slot4;
	}

	TileColors getSlot5() const
	{
		return slot5;
	}

	void setSlot5(const TileColors& slot5)
	{
		this->slot5 = slot5;
	}

	TileColors getSlot6() const
	{
		return slot6;
	}

	void setSlot6(const TileColors& slot6)
	{
		this->slot6 = slot6;
	}

	char getSequenceId() const
	{
		return sequenceId;
	}

	void setSequenceId(char sequence_id)
	{
		sequenceId = sequence_id;
	}

	char getPower() const
	{
		return power;
	}

	void setPower(char power)
	{
		this->power = power;
	}

	SetTypes getTileSetType() const
	{
		return tileSetType;
	}

	void setTileSetType(const SetTypes& tile_set_type)
	{
		tileSetType = tile_set_type;
	}

};


