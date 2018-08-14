#pragma once
#include "cocos2d.h"

#include <string>
#include <list>
#include "General/EnumTypes.h"
#include "General/TypeDefs.h"
#include "Models/DataModels/CustomSpawnTableItem.h"
#include "Cell.h"
#include "Models/Tiles/FixTiles.h"
#include "PieceSwapper.h"
#include "Models/Tiles/FruitRollGroup.h"

class BoardLayerModel;
class LavaCakeObject;

class BoardModel : public cocos2d::Ref
{
public:
	~BoardModel();
	
	static TileColorsTable CreateColorsTableFromJson(const rapidjson::Value& json);
	static std::map<std::string, CustomSpawnTableItem>* CreateCustomSpawnTablesListFromJson(rapidjson::Value& json);
	
	
	void CreateSpawnTableFromJson(rapidjson::Value& json);
	void initSpawners();
	void setNoShuffleCells(rapidjson::Value& json);
	void initWithJson(rapidjson::Value& json);
	void addLayerWithJson(rapidjson::Value& json, LayerId layerNumber);
	BoardModel();
	void initCells();
	Cell* getTurnCell(LayerId layer, GridPos& refPos, AdjacentDirs inDir, AdjacentDirs* newDir, bool counterClockWise);

	BoardLayerModel* getMatchLayer() const { return reinterpret_cast<BoardLayerModel*>(boardLayers->objectForKey(LayerId::Match)); }

	float getCurrentLiquidLevel() const { return currentLiquidLevel; }
	bool isLiquidFull() const { return liquidSystem != nullptr ? currentLiquidLevel >= liquidSystem->LevelMax : false; }
	void setCurrentLiquidLevel(float liquidLevel);

	Cell* getCell(const char col, const char row) const;

	Cell* getSeekerTarget();
	void removeSeekerTarget(SeekerPriorityObject* target);
	CellsList* getSameColorCells(TileColors tileColor);
	__Dictionary* getSpecialTiles();
	Cell* getRandomCell();
	Vec2 getRandomBoardPosition();

	bool isShuffleNeed();
	void shuffle(BoardLayer* showObjectsLayer);
	bool addAvailableMove();
	void clearAvailableMove();
	bool checkAvailableMove(char col, char row);

	bool inBoard(char col, char row) const { return col >= 0 && col < width && row >= 0 && row < height; }

	std::list<PortalInletObject*>* portalInList = nullptr;
	std::list<PortalOutletObject*>* portalOutList = nullptr;
	CellsList* findAvailableMoveCells();
	CellsList* availableMove = nullptr;

	Cell* getDirectFallCell(Cell* cell);
	Cell* getInclinedFallCell(Cell* cell);
	CellsList* getSeekerTargets(int count) const;
	std::list<ConveyorInfo*>* getConveyorInfo() const { return conveyorInfo; }
	void buildConveyors();
	bool containedInConveyors(Cell* cell) const;
	bool checkConveyorStatus(CellsList* conveyor) const;
	ConveyorInfo* findConveyorInfo(Cell* cell) const;
	void conveyTile(MovingTile* from, Cell* to) const;

	CellsList* getLavaCakeTargets();
	void setIncreaseLavaCakeFlag(bool flag = true);
	void addLavaCakeTile(LavaCakeObject* lavaCake);

	void addFruitGroup(FruitRollGroup* fruitGroup);
	void initFruitRollFlags();

	void addObjectSpinnerCell(Cell* cell);
	void runObjectSpinner();
	void rotateSpinner(Cell* cell, bool isClockWise);
	MovingTile* moveTile(Cell* cell, MovingTile* movingTile);

	void moveConveyors();

	void addPieceSwapperCell(std::string color, Cell* cell);
	void runSwappers();

	char getPathMoversCount();
	char getSpecialTilesCount(MovingTileTypes tileType) const;

	Vec2 getBoardCenterPos() const { return Vec2(CellSize * width / 2, CellSize * height / 2); }
	bool containsPortalOutInCol(Cell* cell) const;

	bool containsHoneyModifier() const { return containsHoney; }
	Cell* findHoneyTarget();
	CellsList findNearbyMovingTiles(Cell* cell);

	std::list<FruitRollGroup*>* getFruitRollGroups() const { return fruitRollGroups; }
	std::map<std::string, Cell*>* getHiderTargets() const { return hiderTargets; }
	void removeHiderTarget(std::string chainName) const { hiderTargets->erase(chainName); }

private:
	cocos2d::__Dictionary* boardLayers;
	int width;
	int height;

	bool spawnTop = true;
	bool spawnBottom = false;
	std::string transitionOut;
	SpawnTablesList spawnTable = nullptr;
	SpawnTablesList conveyorSpawnTable;
	TileColorsTable colors;
	TileColorsTable colorsEasy;
	std::list<Goal>* goals;
	std::map<std::string, CustomSpawnTableItem>* customSpawnTable;
	std::list<SeekerPriorityObject*>* seekerPriorityList = nullptr;

	CellsList* lavaCakeTargets = nullptr;

	SpawnTablesList liquidSpawnTable = nullptr;
	cocos2d::__Array* data;
	LiquidSystem* liquidSystem = nullptr;
	std::list<ConveyorInfo*>* conveyorInfo = nullptr;

	std::list<CellsList*>* conveyors = nullptr;

	Cell*** cells;
	float currentLiquidLevel = 0;
	bool hasToAddSpawners = true;

	std::list<LavaCakeObject*>* lavaCakeTiles = nullptr;
	std::list<FruitRollGroup*>* fruitRollGroups = nullptr;
	CellsList* objectSpinnerCells = nullptr;
	std::map<std::string, PieceSwapper*> * pieceSwappers = nullptr;
	SpawnOnCollectSystem* spawnOnCollectSystem = nullptr;
	std::map<std::string, Cell*>* hiderTargets = nullptr;

	bool containsHoney = false;

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

	std::map<std::string, CustomSpawnTableItem>* getCustomSpawnTable() const
	{
		return customSpawnTable;
	}

	void setCustomSpawnTable(std::map<std::string, CustomSpawnTableItem>* custom_spawn_table_items)
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

	SpawnOnCollectSystem* getSpawnOnCollectSystem() const
	{
		return spawnOnCollectSystem;
	}

	void setSpawnOnCollectSystem(SpawnOnCollectSystem* spawnSys)
	{
		spawnOnCollectSystem = spawnSys;
	}
};

