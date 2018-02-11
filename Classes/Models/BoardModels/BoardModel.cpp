#include "deprecated/CCDictionary.h"
#include "BoardModel.h"
#include "Controllers/SpawnController.h"
#include "General/Utils.h"
#include "Controllers/PoolController.h"
#include "Models/Tiles/SpawnerObject.h"
#include "Controllers/ActionController.h"
#include "Controllers/BoardController.h"


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

Cell* BoardModel::getDirectFallCell(Cell* cell)
{
	//if (cell->containsPortalOut() && !cell->inWater)
	//{
	//	return cell->findPortalInCell(portalInList);
	//}
	if (cell->containsSpawner())
	{
		return cell;
	}

	auto loopCell = cell->getFallUpCell();
	auto prevCell = cell;

	while (loopCell != nullptr && loopCell->canFill())
	{
		if (loopCell->canFall())
		{
			return loopCell;
		}
		//if (loopCell->containsPortalOut())
		//{
		//	prevCell = loopCell;
		//	loopCell = loopCell->findPortalInCell(portalInList);
		//	continue;
		//}
		if (loopCell->containsSpawner())
		{
			return loopCell;
		}

		prevCell = loopCell;
		loopCell = loopCell->getFallUpCell();
	}

	if (loopCell != nullptr && loopCell->canFall()) 
	{
		return loopCell;
	}
	return prevCell;
}

Cell* BoardModel::getInclinedFallCell(Cell* cell) 
{
	char inWater = cell->inWater ? -1 : 1;
	auto col = cell->gridPos.Col;
	auto row = cell->gridPos.Row + inWater;

	if (row < 0 || row >= height) 
	{
		return nullptr;
	}

	auto leftCell = col - 1 >= 0 ?  cells[row][col - 1] : nullptr;
	auto rightCell = col + 1 < width ? cells[row][col + 1] : nullptr;

	if (leftCell != nullptr && leftCell->canFall())
	{
		return leftCell;
	}
	if (rightCell != nullptr && rightCell->canFall())
	{
		return rightCell;
	}
	if (leftCell != nullptr && leftCell->canFill())
	{
		return leftCell;
	}
	if (rightCell != nullptr && rightCell->canFill())
	{
		return rightCell;
	}
	return nullptr;
}

Cell* BoardModel::getSeekerTarget()
{
	if(seekerPriorityList == nullptr || seekerPriorityList->size() == 0)
	{
		return nullptr;
	}
	seekerPriorityList->sort([](SeekerPriorityObject* first, SeekerPriorityObject* second){
		if(first->getPriority() < second->getPriority())
		{
			return true;
		}
		if(first->getPriority() > second->getPriority())
		{
			return false;
		}
		if(first->getLayers() > second->getLayers())
		{
			return true;
		}
		return false;
	});
	auto seekerTarget = seekerPriorityList->front();
	seekerTarget->countDownLayer();
	auto retCell = seekerTarget->getCell();
	if(seekerTarget->getLayers() < 0)
	{
		seekerPriorityList->pop_front();
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
			if(cells[i][j] != nullptr && !cells[i][j]->isOutCell && !cells[i][j]->isEmpty && cells[i][j]->getMovingTile()->getTileColor() == tileColor 
				&& cells[i][j]->getMovingTile()->getMovingTileType() != +MovingTileTypes::RainbowObject)
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
	for (char i = 0; i < height; i++)
	{
		for (char j = 0; j < width; j++)
		{
			if(i < currentLiquidLevel && !liquidSystem->containsIgnorePos(j, i) && !liquidSystem->containsIgnoreColumn(j) && !liquidSystem->containsIgnoreRow(i))
			{
				cells[i][j]->inWater = true;
				cells[i][j]->fallDirection = Direction::N;
			}
			else
			{
				cells[i][j]->inWater = false;
				cells[i][j]->fallDirection = Direction::S;
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

void BoardModel::CreateSpawnTableFromJson(rapidjson::Value& json)
{
	spawnTable = CustomSpawnTableItem::CreateSpawnTablesFromJson(json);
	SpawnController::getInstance()->setSpawnTable(spawnTable);
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
				if(!cell->containsSpawner() && !cell->inWater)
				{
					auto spawner = SpawnerObject::create();
					spawner->initSpawner();
					cell->setTileToLayer(spawner, LayerId::Spawner);
				}
				break;
			}
		}
		for (char i = 0; i <= height; i++)
		{
			auto cell = cells[i][j];
			if (!cell->isOutCell)
			{
				if (!cell->containsSpawner() && cell->inWater)
				{
					auto spawner = SpawnerObject::create();
					spawner->initSpawner();
					spawner->setDirection(Direction::N);
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
	CreateSpawnTableFromJson(json["spawn_table"]);

	auto& layersJson = json["layers"];
	for (auto itr = layersJson.MemberBegin(); itr != layersJson.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			const auto layerIndex = atoi(itr->name.GetString());
			if (LayerId::_is_valid(layerIndex))
			{
				const auto layerId = LayerId::_from_integral(layerIndex);
				addLayerWithJson(itr->value, layerId);
			}
		}
		//else if(itr->name == "8")
		//{
		//	initSpawners();
		//}
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

				if (strcmp(dataType, "LiquidSystem") == 0)
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
					setCurrentLiquidLevel(liquidSystem->LevelStart);
				}
			}
		}
	}
	if(hasToAddSpawners)
	{
		initSpawners();
	}

	setNoShuffleCells(json["noReshuffle"]);
}

void BoardModel::setNoShuffleCells(rapidjson::Value& json)
{
	for (auto itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		if (itr->value.IsBool())
		{
			auto gridPos = Utils::StrToGridPos(itr->name.GetString(), "_");
			assert(this->width > gridPos.Col && this->height > gridPos.Row);
			cells[gridPos.Row][gridPos.Col]->noShuffleCell = itr->value.GetBool();
		}
	}

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
							seekerPriorityList = new std::list<SeekerPriorityObject*>;
						}
						seekerPriorityList->push_back(static_cast<SeekerPriorityObject*>(tile));
					}
					else if (strcmp(typeName, "EmptyObject") == 0)
					{
						cells[gridPos.Row][gridPos.Col]->isOutCell = true;
					}
					else if (strcmp(typeName, "InvisibleBrickObject") == 0)
					{
						cells[gridPos.Row][gridPos.Col]->isOutCell = true;
					}
					else if(strcmp(typeName, "PortalOutletObject") == 0)
					{
						if(portalOutList == nullptr)
						{
							portalOutList = new std::list<PortalOutletObject*>;
						}
						portalOutList->push_back(reinterpret_cast<PortalOutletObject* const&>(tile));
					}
					else if (strcmp(typeName, "PortalInletObject") == 0)
					{
						if (portalInList == nullptr)
						{
							portalInList = new std::list<PortalInletObject*>;
						}
						portalInList->push_back(reinterpret_cast<PortalInletObject* const&>(tile));
					}
					//else if(strcmp(typeName, "SpawnerObject") == 0)
					//{
					//	hasToAddSpawners = false;
					//}
				}
			}
		}
	}

}

std::list<Cell*>* BoardModel::findAvailableMoveCells()
{
	for (char i = 0; i < height; i++)
	{
		for (char j = 0; j < width; j++)
		{
			auto cell = cells[i][j];
			if (cell == nullptr || cell->isOutCell || cell->isFixed || cell->isEmpty)
			{
				continue;
			}
			if(!cell->isEmpty && cell->getMovingTile() != nullptr && cell->getMovingTile()->getMovingTileType() != +MovingTileTypes::LayeredMatchObject)
			{
				continue;
			}
			for(char k = 0; k < AVAILABLE_MOVES_COUNT; k++)
			{
				auto allMovable = true;
				for(char l = 0; l < 4; l++)
				{
					auto aCol = j + AvailableMoves[k][l][1];
					auto aRow = i + AvailableMoves[k][l][0];
					if(aCol < 0 || aCol >= width || aRow < 0 || aRow >= height)
					{
						allMovable = false;
						break;
					}
					auto cell1 = cells[aRow][aCol];
					if (cell1 == nullptr || cell1->isOutCell || cell1->isFixed || cell->isEmpty)
					{
						allMovable = false;
						break;
					}
					if (cell1->getMovingTile() == nullptr || cell1->getMovingTile()->getMovingTileType() != +MovingTileTypes::LayeredMatchObject)
					{
						allMovable = false;
						break;
					}
				}

				if(allMovable)
				{
					return new std::list<Cell*>{
						cells[i][j],
						cells[i + AvailableMoves[k][0][0]][j + AvailableMoves[k][0][1]],
						cells[i + AvailableMoves[k][1][0]][j + AvailableMoves[k][1][1]]
					};
				}
			}
		}
	}
	return nullptr;
}

bool BoardModel::addAvailableMove()
{
	auto tileColor = SpawnController::getInstance()->getSpawnColor();
	if(availableMove != nullptr)
	{
		CC_SAFE_DELETE(availableMove);
	}

	availableMove = findAvailableMoveCells();
	if (availableMove == nullptr)
	{
		return false;
	}
	MovingTileTypes tileType = MovingTileTypes::LayeredMatchObject;
	for (auto cell : *availableMove)
	{
		cell->getMovingTile()->initWithType(tileType._to_string(), tileColor);
	}
	return true;
}

void BoardModel::clearAvailableMove()
{
	if(availableMove != nullptr)
	{
		for(auto cell : *availableMove)
		{
			if(!cell->isEmpty && cell->getMovingTile() != nullptr)
			{
				cell->getMovingTile()->initMovingTile();
			}
		}
		CC_SAFE_DELETE(availableMove);
	}
}

bool BoardModel::checkAvailableMove(char col, char row)
{
	auto refCell = cells[row][col];
	if (availableMove != nullptr) CC_SAFE_DELETE(availableMove);
	if (refCell == nullptr || (!refCell->canMatch() && !refCell->isRainbowCell()))
	{
		return false;
	}
	auto refTile = refCell->getMovingTile();
	if(refTile->isBonusTile() && !refCell->isFixed)
	{
		for(char i = 0; i < 4; i++)
		{
			auto aCol = col + AdjacentIndents[i][1];
			auto aRow = row + AdjacentIndents[i][0];
			if(!inBoard(aCol, aRow))
			{
				continue;
			}
			auto aCell = cells[aRow][aCol];
			if(aCell != nullptr && aCell->canMatch())
			{
				if(refTile->getMovingTileType() == +MovingTileTypes::RainbowObject 
					|| aCell->getMovingTile()->isBonusTile())
				{
					availableMove = new std::list<Cell*>{
						refCell,
						aCell
					};
					return true;
				}
			}
		}
	}
	else
	{
		TileColors refColor = refCell->getMovingTile()->getTileColor();
		for(char i = 0; i < AVAILABLE_MOVES_COUNT; i++)
		{
			std::list<Cell*> availableTiles;
			char count = 0;
			for(char j = 0; j < 2; j++)
			{
				auto aCol = col + AvailableMoves[i][j][1];
				auto aRow = row + AvailableMoves[i][j][0];
				if(!inBoard(aCol, aRow))
				{
					continue;
				}
				auto aCell = cells[aRow][aCol];
				if(aCell != nullptr && aCell->canMatch())
				{
					if(aCell->getMovingTile()->getTileColor() == refColor)
					{
						count++;
						availableTiles.push_back(aCell);
					}
				}
			}

			if(count >= 2)
			{
				auto bCol = col + AvailableMoves[i][2][1];
				auto bRow = row + AvailableMoves[i][2][0];
				auto cCol = col + AvailableMoves[i][3][1];
				auto cRow = row + AvailableMoves[i][3][0];
				if(!inBoard(bCol, bRow) || !inBoard(bCol, bRow))
				{
					continue;
				}
				auto bCell = cells[bRow][bCol];
				auto cCell = cells[cRow][cCol];
				if (bCell != nullptr && bCell->canMove() && cCell != nullptr && cCell->canMove())
				{
					availableMove = new std::list<Cell*>(availableTiles);
					availableMove->push_back(refCell);
					return true;
				}
			}
		}
	}

	return false;
}


bool BoardModel::isShuffleNeed()
{
	for(char i = 0; i < height; i++)
	{
		for(char j = 0; j < width; j++)
		{
			if(checkAvailableMove(j, i))
			{
				return false;
			}
		}
	}
	return true;
}

void BoardModel::shuffle(BoardLayer* showObjectsLayer)
{
	MovingTile** shuffleTiles = new MovingTile*[height*width];
	char shuffleTilesCount = 0;
	for (char i = 0; i < height; i++)
	{
		for (char j = 0; j < width; j++)
		{
			if (cells[i][j] != nullptr && !cells[i][j]->isNoShuffleCell())
			{
				shuffleTiles[shuffleTilesCount] = cells[i][j]->getMovingTile();
				shuffleTilesCount++;
			}
		}
	}
	auto shufflePos = static_cast<char>(rand_0_1() * shuffleTilesCount);

	for(char i = 0; i < height; i++)
	{
		for(char j = 0; j < width; j++)
		{
			if(!cells[i][j]->isNoShuffleCell())
			{
				auto movingTile = shuffleTiles[shufflePos];
				movingTile->setVisible(false);
				auto showObj = PoolController::getInstance()->getTileShowObject();
				showObj->setSpriteFrame(movingTile->getMainSpriteFrame());
				showObj->setPosition(movingTile->getPosition());
				showObj->setAnchorPoint(Vec2(0.5, 0.5));
				if (showObj != nullptr && showObj->getParent() == nullptr)
				{
					showObjectsLayer->addChild(showObj);
				}

				auto actionController = ActionController::getInstance();
				CKAction ckAction;
				ckAction.node = reinterpret_cast<Node*>(showObj);

				ckAction.action = ActionController::getInstance()->createShuffleMoveAction(
					getBoardCenterPos(),
					cells[i][j]->getBoardPos(),
					[this, movingTile, showObj]()
					{
						movingTile->setVisible(true);
						movingTile->isMoving = false;
						PoolController::getInstance()->recycleTileShowObject(showObj);
						BoardController::gameState = GameState::Idle;
					}, ckAction.node);

				//ckAction.delayCount = actionCount / 3;
				movingTile->setVisible(false);
				movingTile->isMoving = true;
				actionController->pushAction(ckAction, true);

				cells[i][j]->createShuffleShow();
				shuffleTiles[shufflePos]->getCell()->shuffleResultCell = cells[i][j];
				cells[i][j]->setSourceTile(shuffleTiles[shufflePos]);
				cells[i][j]->getMovingTile()->setPosition(cells[i][j]->getBoardPos());
				shufflePos++;
				if(shufflePos >= shuffleTilesCount)
				{
					shufflePos = 0;
				}
			}
		}
	}
	CC_SAFE_DELETE(shuffleTiles);
}