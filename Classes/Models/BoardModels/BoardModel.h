#pragma once
#include "cocos2d.h"

#include <string>
#include <list>
#include "General/EnumTypes.h"
#include "General/TypeDefs.h"
#include "Models/DataModels/CustomSpawnTableItem.h"

class BoardLayerModel;

class BoardModel : public cocos2d::Ref
{
public:
	~BoardModel();
	static TileColorsTable CreateColorsTableFromJson(const rapidjson::Value& json);
	static std::list<CustomSpawnTableItem>* CreateCustomSpawnTablesListFromJson(rapidjson::Value& json);
	void initWithJson(rapidjson::Value& json);
	BoardModel();

	BoardLayerModel* getMatchLayer() const { return reinterpret_cast<BoardLayerModel*>(boardLayers->objectForKey(LayerId::Match)); }

private:

public:
	rapidjson::Document LayersJson;

private:
	cocos2d::__Dictionary* boardLayers;
	int width;
	int height;
	std::string transitionOut;
	SpawnTablesList spawnTable;
	SpawnTablesList conveyorSpawnTable;
	TileColorsTable colors;
	TileColorsTable colorsEasy;
	std::list<Goal>* goals;
	std::list<CustomSpawnTableItem>* customSpawnTable;
	SpawnTablesList liquidSpawnTable;
	cocos2d::__Array* data;


#pragma region Get Set Methods

public:
	cocos2d::__Dictionary* layers2() const
	{
		return boardLayers;
	}

	void layers2(cocos2d::__Dictionary* layers)
	{
		this->boardLayers = layers;
	}

	__declspec(property(get = layers2, put = layers2)) cocos2d::__Dictionary* BoardLayers;

	std::list<Goal>* goals1() const
	{
		return goals;
	}

	void goals1(std::list<Goal>* goals)
	{
		this->goals = goals;
	}

	__declspec(property(get = goals1, put = goals1)) std::list<Goal>* Goals;


	std::list<CustomSpawnTableItem>* custom_spawn_table() const
	{
		return customSpawnTable;
	}

	void custom_spawn_table(std::list<CustomSpawnTableItem>* customSpawnTableItems)
	{
		customSpawnTable = customSpawnTableItems;
	}

	__declspec(property(get = custom_spawn_table, put = custom_spawn_table)) std::list<CustomSpawnTableItem>*
	CustomSpawnTable;

	int width1() const
	{
		return width;
	}

	void width1(int width)
	{
		this->width = width;
	}

	__declspec(property(get = width1, put = width1)) int Width;

	int height1() const
	{
		return height;
	}

	void height1(int height)
	{
		this->height = height;
	}

	__declspec(property(get = height1, put = height1)) int Height;

	std::string transition_out() const
	{
		return transitionOut;
	}

	void transition_out(const std::string& transitionOut)
	{
		this->transitionOut = transitionOut;
	}

	__declspec(property(get = transition_out, put = transition_out)) std::string TransitionOut;

	SpawnTablesList spawn_table() const
	{
		return spawnTable;
	}

	void spawn_table(const SpawnTablesList spawnTables)
	{
		spawnTable = spawnTables;
	}

	__declspec(property(get = spawn_table, put = spawn_table)) SpawnTablesList SpawnTableData;

	SpawnTablesList conveyor_spawn_table() const
	{
		return conveyorSpawnTable;
	}

	void conveyor_spawn_table(const SpawnTablesList conveyorSpawnTable)
	{
		this->conveyorSpawnTable = conveyorSpawnTable;
	}

	__declspec(property(get = conveyor_spawn_table, put = conveyor_spawn_table)) SpawnTablesList ConveyorSpawnTable;

	TileColorsTable colors1() const
	{
		return colors;
	}

	void colors1(const TileColorsTable colors)
	{
		this->colors = colors;
	}

	__declspec(property(get = colors1, put = colors1)) TileColorsTable Colors;

	TileColorsTable colors_easy() const
	{
		return colorsEasy;
	}

	void colors_easy(const TileColorsTable colorsEasy)
	{
		this->colorsEasy = colorsEasy;
	}

	__declspec(property(get = colors_easy, put = colors_easy)) TileColorsTable ColorsEasy;


	SpawnTablesList liquid_spawn_table() const
	{
		return liquidSpawnTable;
	}

	void liquid_spawn_table(const SpawnTablesList liquidSpawnTable)
	{
		this->liquidSpawnTable = liquidSpawnTable;
	}

	__declspec(property(get = liquid_spawn_table, put = liquid_spawn_table)) SpawnTablesList LiquidSpawnTable;

	cocos2d::__Array* data1() const
	{
		return data;
	}

	void data1(cocos2d::__Array* data)
	{
		this->data = data;
	}

	__declspec(property(get = data1, put = data1)) cocos2d::__Array* Data;
#pragma endregion

};

