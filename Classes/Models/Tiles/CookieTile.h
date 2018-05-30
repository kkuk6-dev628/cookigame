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

	virtual SpriteFrame* getSpriteFrame() { return textureSprite == nullptr ? nullptr : textureSprite->getSpriteFrame(); }

	virtual bool isMovable() { return true; }
	virtual bool crush(bool showEffect);

	void setNearbyColor(TileColors color) { nearbyColor = color; }

	bool canMatch = true;
	int matchId = 0;
	bool receiveNearbyAffect = false;
	bool noClear = false;

protected:
	Cell* pCell;
	TileColors nearbyColor = TileColors::any;

	std::string type;
	char layers = 0;
	TileColors color = TileColors::any;
	Direction direction = Direction::any;
	std::string dirString;
	std::string name;
	char priority = 0;

	bool createSpread = false;
	bool mirror = false;
	ModifierTypes modifierType = ModifierTypes::None;
	std::string gridSize;
	SetTypes tileSetType = SetTypes::set;
	bool createJelly = false;
	HiderSegmentPosition hiderSegPosition = HiderSegmentPosition::body;
	std::string positionString;
	std::string chain;
	std::string spawnType;
	SpawnTable spawnData;
	char zone = 0;
	std::string spawnTableName;
	std::list<TileColors>* slotColors = nullptr;
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

	Direction getDirection() const
	{
		return direction;
	}

	void setDirection(const Direction& direction)
	{
		this->direction = direction;
	}

	std::string getDirectionString() const
	{
		return dirString;
	}

	void setDirectionString(const std::string& direction)
	{
		this->dirString = direction;
	}

	std::string getTileName() const
	{
		return name;
	}

	void setTileName(const std::string& name)
	{
		this->name = name;
	}

	std::string getPositionString() const
	{
		return positionString;
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


