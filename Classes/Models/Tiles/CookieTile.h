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
	virtual void initWithJson(rapidjson::Value& json);

	int matchId = 0;

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
	SetTypes setType = SetTypes::set;
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


#pragma region Getter and Setter

public:
	std::string type1() const
	{
		return type;
	}

	void type1(const std::string& type)
	{
		this->type = type;
	}

	__declspec(property(get = type1, put = type1)) std::string Type;

	char layers1() const
	{
		return layers;
	}

	void layers1(char layers)
	{
		this->layers = layers;
	}

	__declspec(property(get = layers1, put = layers1)) char Layers;

	TileColors color1() const
	{
		return color;
	}

	void color1(const TileColors& color)
	{
		this->color = color;
	}

	__declspec(property(get = color1, put = color1)) TileColors Color;

	std::string direction1() const
	{
		return direction;
	}

	void direction1(const std::string& direction)
	{
		this->direction = direction;
	}

	__declspec(property(get = direction1, put = direction1)) std::string Direction;

	std::string name1() const
	{
		return name;
	}

	void name1(const std::string& name)
	{
		this->name = name;
	}

	__declspec(property(get = name1, put = name1)) std::string Name;

	char priority1() const
	{
		return priority;
	}

	void priority1(char priority)
	{
		this->priority = priority;
	}

	__declspec(property(get = priority1, put = priority1)) char Priority;

	bool create_spread() const
	{
		return createSpread;
	}

	void create_spread(bool createSpread)
	{
		this->createSpread = createSpread;
	}

	__declspec(property(get = create_spread, put = create_spread)) bool CreateSpread;

	bool mirror1() const
	{
		return mirror;
	}

	void mirror1(bool mirror)
	{
		this->mirror = mirror;
	}

	__declspec(property(get = mirror1, put = mirror1)) bool Mirror;

	ModifierTypes modifier_type() const
	{
		return modifierType;
	}

	void modifier_type(const ModifierTypes& modifierType)
	{
		this->modifierType = modifierType;
	}

	__declspec(property(get = modifier_type, put = modifier_type)) ModifierTypes ModifierType;

	GridSizeT grid_size() const
	{
		return gridSize;
	}

	void grid_size(const GridSizeT& gridSize)
	{
		this->gridSize = gridSize;
	}

	__declspec(property(get = grid_size, put = grid_size)) GridSizeT GridSize;

	SetTypes set_type() const
	{
		return setType;
	}

	void set_type(const SetTypes& setType)
	{
		this->setType = setType;
	}

	__declspec(property(get = set_type, put = set_type)) SetTypes SetType;

	bool create_jelly() const
	{
		return createJelly;
	}

	void create_jelly(bool createJelly)
	{
		this->createJelly = createJelly;
	}

	__declspec(property(get = create_jelly, put = create_jelly)) bool CreateJelly;

	HiderSegmentPosition hider_seg_position() const
	{
		return hiderSegPosition;
	}

	void hider_seg_position(const HiderSegmentPosition& hiderSegPosition)
	{
		this->hiderSegPosition = hiderSegPosition;
	}

	__declspec(property(get = hider_seg_position, put = hider_seg_position)) HiderSegmentPosition HiderSegPosition;

	std::string chain1() const
	{
		return chain;
	}

	void chain1(const std::string& chain)
	{
		this->chain = chain;
	}

	__declspec(property(get = chain1, put = chain1)) std::string Chain;

	std::string spawn_type() const
	{
		return spawnType;
	}

	void spawn_type(const std::string& spawnType)
	{
		this->spawnType = spawnType;
	}

	__declspec(property(get = spawn_type, put = spawn_type)) std::string SpawnType;

	SpawnTable spawn_data() const
	{
		return spawnData;
	}

	void spawn_data(const SpawnTable& spawnData)
	{
		this->spawnData = spawnData;
	}

	__declspec(property(get = spawn_data, put = spawn_data)) SpawnTable SpawnData;

	char zone1() const
	{
		return zone;
	}

	void zone1(char zone)
	{
		this->zone = zone;
	}

	__declspec(property(get = zone1, put = zone1)) char Zone;

	std::string spawn_table_name() const
	{
		return spawnTableName;
	}

	void spawn_table_name(const std::string& spawnTableName)
	{
		this->spawnTableName = spawnTableName;
	}

	__declspec(property(get = spawn_table_name, put = spawn_table_name)) std::string SpawnTableName;

	TileColors slot7() const
	{
		return slot1;
	}

	void slot7(const TileColors& slot1)
	{
		this->slot1 = slot1;
	}

	__declspec(property(get = slot7, put = slot7)) TileColors Slot1;

	TileColors slot2_1() const
	{
		return slot2;
	}

	void slot2_1(const TileColors& slot2)
	{
		this->slot2 = slot2;
	}

	__declspec(property(get = slot2_1, put = slot2_1)) TileColors Slot2;

	TileColors slot3_1() const
	{
		return slot3;
	}

	void slot3_1(const TileColors& slot3)
	{
		this->slot3 = slot3;
	}

	__declspec(property(get = slot3_1, put = slot3_1)) TileColors Slot3;

	TileColors slot4_1() const
	{
		return slot4;
	}

	void slot4_1(const TileColors& slot4)
	{
		this->slot4 = slot4;
	}

	__declspec(property(get = slot4_1, put = slot4_1)) TileColors Slot4;

	TileColors slot5_1() const
	{
		return slot5;
	}

	void slot5_1(const TileColors& slot5)
	{
		this->slot5 = slot5;
	}

	__declspec(property(get = slot5_1, put = slot5_1)) TileColors Slot5;

	TileColors slot6_1() const
	{
		return slot6;
	}

	void slot6_1(const TileColors& slot6)
	{
		this->slot6 = slot6;
	}

	__declspec(property(get = slot6_1, put = slot6_1)) TileColors Slot6;

	char sequence_id() const
	{
		return sequenceId;
	}

	void sequence_id(char sequenceId)
	{
		this->sequenceId = sequenceId;
	}

	__declspec(property(get = sequence_id, put = sequence_id)) char SequenceId;

	char power1() const
	{
		return power;
	}

	void power1(char power)
	{
		this->power = power;
	}

	__declspec(property(get = power1, put = power1)) char Power;
#pragma endregion

};

class WaffleObject : public CookieTile
{

};

