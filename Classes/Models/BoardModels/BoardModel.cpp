#include "deprecated/CCDictionary.h"
#include "BoardModel.h"
#include "Controllers/SpawnController.h"
#include "General/Utils.h"
#include "Controllers/PoolController.h"
#include "Models/Tiles/SpawnerObject.h"


BoardModel::BoardModel()
{
	this->boardLayers = cocos2d::__Dictionary::create();
	this->boardLayers->retain();

}

void BoardModel::initCells()
{
	cells = new Cell**[height];
	for (auto i = 0; i < height; i++)
	{
		cells[i] = new Cell*[width];

		for (auto j = 0; j < width; j++)
		{
			cells[i][j] = new Cell();
			cells[i][j]->setGridPos(j, i);
			if (i > 0)
			{
				cells[i - 1][j]->upCell = cells[i][j];
				cells[i][j]->downCell = cells[i - 1][j];
			}
			if (j > 0)
			{
				cells[i][j - 1]->rightCell = cells[i][j];
				cells[i][j]->leftCell = cells[i][j - 1];
			}
		}
	}
}

Cell* BoardModel::getTurnCell(LayerId layer, GridPos& refPos, AdjacentDirs inputDir, AdjacentDirs* newDir, bool counterClockWise)
{
	if (counterClockWise)
	{
		switch (inputDir)
		{
		case AdjacentDirs::E:
			*newDir = AdjacentDirs::N;
			return getCell(refPos.Col, refPos.Row - 1);
		case AdjacentDirs::W:
			*newDir = AdjacentDirs::S;
			return getCell(refPos.Col, refPos.Row + 1);
		case AdjacentDirs::N:
			*newDir = AdjacentDirs::W;
			return getCell(refPos.Col + 1, refPos.Row);
		case AdjacentDirs::S:
			*newDir = AdjacentDirs::E;
			return getCell(refPos.Col - 1, refPos.Row);
		}
		return nullptr;
	}
	else
	{
		switch (inputDir)
		{
		case AdjacentDirs::E:
			*newDir = AdjacentDirs::S;
			return getCell(refPos.Col, refPos.Row + 1);
		case AdjacentDirs::W:
			*newDir = AdjacentDirs::N;
			return getCell(refPos.Col, refPos.Row - 1);
		case AdjacentDirs::N:
			*newDir = AdjacentDirs::E;
			return getCell(refPos.Col - 1, refPos.Row);
		case AdjacentDirs::S:
			*newDir = AdjacentDirs::W;
			return getCell(refPos.Col + 1, refPos.Row);
		}
		return nullptr;
	}
}

Cell* BoardModel::getSeekerTarget()
{
	if(seekerPriorityList == nullptr || seekerPriorityList->count() == 0)
	{
		return nullptr;
	}
	auto seekerTarget = static_cast<SeekerPriorityObject*>(seekerPriorityList->getRandomObject());
	seekerTarget->countDownLayer();
	auto retCell = seekerTarget->getCell();
	if(seekerTarget->getLayers() < 0)
	{
		seekerPriorityList->removeObject(seekerTarget);
		seekerTarget->release();
	}
	return retCell;
}

std::list<Cell*>* BoardModel::getSameColorCells(TileColors tileColor)
{
	auto ret = new std::list<Cell*>;
	for (char i = 0; i < height; i++)
	{
		for (char j = 0; j < width; j++)
		{
			if(cells[i][j] != nullptr && !cells[i][j]->isOutCell && !cells[i][j]->isEmpty && cells[i][j]->getMovingTile()->getTileColor() == tileColor)
			{
				ret->push_back(cells[i][j]);
			}
		}
	}
	return ret;
}

__Dictionary* BoardModel::getSpecialTiles()
{
	auto specialTiles = __Dictionary::create();
	auto breakers = __Array::create();
	auto wafflesAndPathMovers = __Array::create();
	auto liquids = __Array::create();

	for(char i = 0; i < height; i++)
	{
		for(char j = 0; j < width; j++)
		{
			if(cells[i][j] == nullptr || cells[i][j]->isEmpty) continue;

			auto tile = cells[i][j]->getSourceTile();
			if(tile == nullptr) continue;

			if(tile->getType() == "BombBreakerObject" || tile->getType() == "ColumnBreakerObject" || tile->getType() == "RowBreakerObject" || 
				tile->getType() == "XBreakerObject")
			{
				breakers->addObject(tile->getCell());
			}
			if (tile->getType() == "WaffleObject" || tile->getType() == "PathMoverMatchObject")
			{
				wafflesAndPathMovers->addObject(tile->getCell());
			}
			if (tile->getType() == "LiquidDrainerMatchObject" || tile->getType() == "LiquidFillerMatchObject")
			{
				liquids->addObject(tile->getCell());
			}
		}
	}

	specialTiles->setObject(breakers, "breakers");
	specialTiles->setObject(wafflesAndPathMovers, "wafflePath");
	specialTiles->setObject(liquids, "liquids");
	return specialTiles;
}

Cell* BoardModel::getRandomCell()
{
	auto col = static_cast<char>(rand_0_1() * width);
	auto row = static_cast<char>(rand_0_1() * height);
	while(cells[row][col] == nullptr || cells[row][col]->isOutCell || cells[row][col]->isEmpty)
	{
		col = static_cast<char>(rand_0_1() * width);
		row = static_cast<char>(rand_0_1() * height);
	}
	return cells[row][col];
}

Vec2 BoardModel::getRandomBoardPosition()
{
	auto dx = width * CellSize * 0.3;
	auto dy = height * CellSize * 0.3;
	auto randX = rand_0_1() * dx + dx;
	auto randY = rand_0_1() * dy + dy;
	return Vec2(randX, randY);
	//return Vec2(width * CellSize, height* CellSize);
}

void BoardModel::setCurrentLiquidLevel(const float liquidLevel)
{
	if(liquidSystem == nullptr)
	{
		currentLiquidLevel = 0;
		return;
	}
	currentLiquidLevel = liquidLevel;
	for (char i = 0; i < currentLiquidLevel; i++)
	{
		for (char j = 0; j < width; j++)
		{
			if(!liquidSystem->containsIgnorePos(j, i) && !liquidSystem->containsIgnoreColumn(j) && !liquidSystem->containsIgnoreRow(i))
			{
				cells[i][j]->inWater = true;
			}
		}
	}
}

Cell* BoardModel::getCell(const char col, const char row) const
{
	if (col < 0 || col >= width || row < 0 || row >= height)
	{
		return nullptr;
	}
	return cells[row][col];
}


BoardModel::~BoardModel()
{
	this->boardLayers->release();
	if (liquidSystem != nullptr)
	{
		CC_SAFE_DELETE(liquidSystem);
	}

	for (auto i = 0; i < height; i++)
	{
		for (auto j = 0; j < width; j++)
		{
			CC_SAFE_DELETE(cells[i][j]);
		}
		CC_SAFE_DELETE_ARRAY(cells[i]);
	}
	CC_SAFE_DELETE_ARRAY(cells);

}

TileColorsTable BoardModel::CreateColorsTableFromJson(const rapidjson::Value& json)
{
	if (json.IsObject())
	{
		auto ret = new std::map<TileColors, float>();
		for (size_t index = 0; index < TileColors::_size(); ++index) {
			auto color = TileColors::_values()[index];
			if (json.HasMember(color._to_string()))
			{
				ret->insert({ color, json[color._to_string()].GetFloat() });
			}
		}
		return ret;
	}
	return nullptr;
}

std::list<CustomSpawnTableItem>* BoardModel::CreateCustomSpawnTablesListFromJson(rapidjson::Value& json)
{
	if (json.IsArray() && json.Size() > 0)
	{
		auto ret = new std::list<CustomSpawnTableItem>();
		auto arr = json.GetArray();

		for (auto& conv : arr)
		{
			CustomSpawnTableItem spt;
			spt.initWithJson(conv);
			ret->push_back(spt);
		}
		return ret;
	}
	return nullptr;
}

void BoardModel::initSpawners()
{
	for(char j = 0; j < width; j++)
	{
		for(char i = height - 1; i >= 0; i--)
		{
			auto cell = cells[i][j];
			if(!cell->isOutCell)
			{
				if(!cell->containsSpawner())
				{
					auto spawner = SpawnerObject::create();
					cell->setTileToLayer(spawner, LayerId::Spawner);
				}
				break;
			}
		}
	}
}

void BoardModel::initWithJson(rapidjson::Value& json)
{
	assert(json.IsObject());
	this->conveyorSpawnTable = CustomSpawnTableItem::CreateSpawnTablesFromJson(json["conveyor_spawn_table"]);
	this->spawnTable = CustomSpawnTableItem::CreateSpawnTablesFromJson(json["spawn_table"]);
	this->transitionOut = json["transitionOut"].GetString();
	this->width = json["width"].GetInt();
	this->height = json["height"].GetInt();
	if (width > 0 && height > 0)
	{
		initCells();
	}
	this->colors = CreateColorsTableFromJson(json["colors"]);

	SpawnController::getInstance()->setColorTable(colors);

	this->colorsEasy = CreateColorsTableFromJson(json["colorsEasy"]);
	this->goals = new std::list<Goal>();
	auto goalsArray = json["goals"].GetArray();
	for (auto& v : goalsArray)
	{
		Goal goal;
		goal.initWithJson(v);
		this->goals->push_back(goal);
	}

	this->customSpawnTable = CreateCustomSpawnTablesListFromJson(json["custom_spawn_table"]);

	auto& layersJson = json["layers"];
	for (auto itr = layersJson.MemberBegin(); itr != layersJson.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			//auto boardLayer = BoardLayerModel::create(this->width, this->height);
			//boardLayer->initWithJson(itr->value, cells);
			const auto layerIndex = atoi(itr->name.GetString());
			if (LayerId::_is_valid(layerIndex))
			{
				const auto layerId = LayerId::_from_integral(layerIndex);
				addLayerWithJson(itr->value, layerId);
			}
			//this->boardLayers->setObject(boardLayer, layerIndex);
		}
	}

	auto& data = json["data"];
	if (data.IsArray() && data.Size() > 0)
	{
		auto dataArray = data.GetArray();
		for (auto& customData : dataArray)
		{
			if (customData.IsObject() && !customData.ObjectEmpty())
			{
				const auto dataType = customData["type"].GetString();

				if (dataType == "LiquidSystem")
				{
					liquidSystem = new LiquidSystem();
					liquidSystem->TurnTimer = customData["turn_timer"].GetInt();
					liquidSystem->LevelMax = customData["level_max"].GetFloat();
					liquidSystem->FillerToggle = customData["filler_toggle"].GetInt();
					liquidSystem->DrainerToggle = customData["drainer_toggle"].GetInt();
					liquidSystem->DrainStep = customData["drain_step"].GetFloat();
					liquidSystem->FillStep = customData["fill_step"].GetFloat();
					liquidSystem->LevelMin = customData["level_min"].GetFloat();
					liquidSystem->LevelStep = customData["level_step"].GetFloat();
					liquidSystem->LevelStart = customData["level_start"].GetFloat();
					if(customData["ignore_columns"].IsArray() && customData["ignore_columns"].Size() > 0)
					{
						auto ignorColumns = customData["ignore_columns"].GetArray();
						for(auto& col : ignorColumns)
						{
							liquidSystem->IgnoreColumns.push_back(col.GetInt());
						}
					}
					if (customData["ignore_rows"].IsArray() && customData["ignore_rows"].Size() > 0)
					{
						auto ignorRows = customData["ignore_rows"].GetArray();
						for (auto& row : ignorRows)
						{
							liquidSystem->IgnoreRows.push_back(row.GetInt());
						}
					}
					if (customData["ignore_grid_pos"].IsArray() && customData["ignore_grid_pos"].Size() > 0)
					{
						auto ignoreGridPos = customData["ignore_grid_pos"].GetArray();
						for (auto& gridPos : ignoreGridPos)
						{
							auto gridPosStr = gridPos.GetString();
							auto gPos = Utils::StrToGridPos(gridPosStr, ",");
							liquidSystem->IgnoreGridPos.push_back(gPos);
						}
					}
				}
			}
		}
	}
	initSpawners();
}

void BoardModel::addLayerWithJson(rapidjson::Value& json, const LayerId layerNumber)
{
	for (auto itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			auto gridPos = Utils::StrToGridPos(itr->name.GetString(), "_");
			assert(this->width > gridPos.Col && this->height > gridPos.Row);
			const auto itr1 = itr->value.FindMember("type");
			if (itr1 != itr->value.MemberEnd() && itr1->value.IsString())
			{
				const auto typeName = itr1->value.GetString();
				auto tile = PoolController::getInstance()->getCookieTile(typeName);
				if (tile != nullptr)
				{
					tile->initWithGrid(gridPos.Col, gridPos.Row);
					tile->initWithJson(itr->value);

					cells[gridPos.Row][gridPos.Col]->setTileToLayer(tile, layerNumber);

					if(strcmp(typeName, "SeekerPriorityObject") == 0)
					{
						if(seekerPriorityList == nullptr)
						{
							seekerPriorityList = __Array::create();
							seekerPriorityList->retain();
						}
						seekerPriorityList->addObject(tile);
					}
					else if (strcmp(typeName, "EmptyObject") == 0)
					{
						cells[gridPos.Row][gridPos.Col]->isOutCell = true;
					}
					else if (strcmp(typeName, "InvisibleBrickObject") == 0)
					{
						cells[gridPos.Row][gridPos.Col]->isOutCell = true;
					}
				}
			}
		}
	}

}

