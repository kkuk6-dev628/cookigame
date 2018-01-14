#pragma once
#include "cocos2d.h"

#include <string>
#include <list>
#include "General/EnumTypes.h"
#include "General/TypeDefs.h"
#include "Models/DataModels/CustomSpawnTableItem.h"
#include "Cell.h"
#include "Models/Tiles/FixTiles.h"

class BoardLayerModel;

class BoardModel : public cocos2d::Ref
{
public:
	~BoardModel();
	static TileColorsTable CreateColorsTableFromJson(const rapidjson::Value& json);
	static std::list<CustomSpawnTableItem>* CreateCustomSpawnTablesListFromJson(rapidjson::Value& json);
	void initSpawners();
	void initWithJson(rapidjson::Value& json);
	void addLayerWithJson(rapidjson::Value& json, LayerId layerNumber);
	BoardModel();
	void initCells();
	Cell* getTurnCell(LayerId layer, GridPos& refPos, AdjacentDirs inDir, AdjacentDirs* newDir, bool counterClockWise);

	BoardLayerModel* getMatchLayer() const { return reinterpret_cast<BoardLayerModel*>(boardLayers->objectForKey(LayerId::Match)); }

	float getCurrentLiquidLevel() const { return currentLiquidLevel; }
	void setCurrentLiquidLevel(float liquidLevel);

	Cell* getCell(const char col, const char row) const;

	Cell* getSeekerTarget();
	std::list<Cell*>* getSameColorCells(TileColors tileColor);
	__Dictionary* getSpecialTiles();
	Cell* getRandomCell();
	Vec2 getRandomBoardPosition();

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
	cocos2d::__Array* seekerPriorityList = nullptr;
	SpawnTablesList liquidSpawnTable;
	cocos2d::__Array* data;
	LiquidSystem* liquidSystem;

	Cell*** cells;
	float currentLiquidLevel = 0;

public:
	rapidjson::Document LayersJson;

	int getWidth() const
	{
		return width;
	}

	void setWidth(int width)
	{
		this->width = width;
	}

	int getHeight() const
	{
		return height;
	}

	void setHeight(int height)
	{
		this->height = height;
	}

	std::string getTransitionOut() const
	{
		return transitionOut;
	}

	void setTransitionOut(const std::string& transition_out)
	{
		transitionOut = transition_out;
	}

	SpawnTablesList getSpawnTable() const
	{
		return spawnTable;
	}

	void setSpawnTable(SpawnTablesList spawn_tables)
	{
		spawnTable = spawn_tables;
	}

	SpawnTablesList getConveyorSpawnTable() const
	{
		return conveyorSpawnTable;
	}

	void setConveyorSpawnTable(SpawnTablesList conveyor_spawn_table)
	{
		conveyorSpawnTable = conveyor_spawn_table;
	}

	TileColorsTable getColors() const
	{
		return colors;
	}

	void setColors(TileColorsTable colors)
	{
		this->colors = colors;
	}

	TileColorsTable getColorsEasy() const
	{
		return colorsEasy;
	}

	void setColorsEasy(TileColorsTable colors_easy)
	{
		colorsEasy = colors_easy;
	}

	std::list<Goal>* getGoals() const
	{
		return goals;
	}

	void setGoals(std::list<Goal>* goals)
	{
		this->goals = goals;
	}

	std::list<CustomSpawnTableItem>* getCustomSpawnTable() const
	{
		return customSpawnTable;
	}

	void setCustomSpawnTable(std::list<CustomSpawnTableItem>* custom_spawn_table_items)
	{
		customSpawnTable = custom_spawn_table_items;
	}

	SpawnTablesList getLiquidSpawnTable() const
	{
		return liquidSpawnTable;
	}

	void setLiquidSpawnTable(SpawnTablesList liquid_spawn_table)
	{
		liquidSpawnTable = liquid_spawn_table;
	}

	cocos2d::__Array* getData() const
	{
		return data;
	}

	void setData(cocos2d::__Array* data)
	{
		this->data = data;
	}

	LiquidSystem* getLiquidSystem() const
	{
		return liquidSystem;
	}

	void setLiquidSystem(LiquidSystem* liquid_system)
	{
		liquidSystem = liquid_system;
	}

	Cell*** getCells() const
	{
		return cells;
	}

	void setCells(Cell*** cells)
	{
		this->cells = cells;
	}

	float getCurrentLiquidLevel1() const
	{
		return currentLiquidLevel;
	}

	void setCurrentLiquidLevel1(float current_liquid_level)
	{
		currentLiquidLevel = current_liquid_level;
	}


};

