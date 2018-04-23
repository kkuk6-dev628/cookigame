#include "deprecated/CCDictionary.h"
#include "BoardModel.h"
#include "Controllers/SpawnController.h"
#include "General/Utils.h"
#include "Controllers/PoolController.h"
#include "Models/Tiles/SpawnerObject.h"
#include "Controllers/ActionController.h"
#include "Controllers/BoardController.h"
#include "Models/Tiles/ConveyorBeltObject.h"
#include "Models/Tiles/LavaCakeObject.h"

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

	while (loopCell != nullptr && (loopCell->canFill() || (loopCell->canPass && (loopCell->isOutCell || loopCell->isFixed))))
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

void BoardModel::addPieceSwapperCell(std::string color, Cell* cell)
{
	if(pieceSwappers == nullptr)
	{
		pieceSwappers = new std::map<std::string, PieceSwapper*>;
	}
	PieceSwapper* swapper = nullptr;
	if(pieceSwappers->find(color) == pieceSwappers->end())
	{
		swapper = new PieceSwapper;
		pieceSwappers->insert(std::pair<std::string, PieceSwapper*>(color, swapper));
	}
	else
	{
		swapper = pieceSwappers->at(color);
	}
	swapper->addCell(cell);
}

void BoardModel::runSwappers()
{
	if(pieceSwappers == nullptr || pieceSwappers->size() == 0)
	{
		return;
	}
	for(auto swapperPair : *pieceSwappers)
	{
		swapperPair.second->swap();
	}
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
	if(seekerTarget->getLayers() <= 0)
	{
		seekerPriorityList->pop_front();
		//seekerTarget->release();
	}
	return retCell;
}

CellsList* BoardModel::getSameColorCells(TileColors tileColor)
{
	auto ret = new CellsList;
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
	auto thopplers = __Array::create();
	auto liquids = __Array::create();

	for(char i = 0; i < height; i++)
	{
		for(char j = 0; j < width; j++)
		{
			if(cells[i][j] == nullptr || cells[i][j]->isEmpty) continue;

			auto cell = cells[i][j];
			auto tile = cell->getSourceTile();
			if(tile == nullptr) continue;

			if(tile->getType() == "BombBreakerObject" || tile->getType() == "ColumnBreakerObject" || tile->getType() == "RowBreakerObject" || 
				tile->getType() == "XBreakerObject")
			{
				breakers->addObject(cell);
			}
			if (tile->getType() == "PathMoverMatchObject"
				|| cell->containsThoppler() || cell->containsWaffle())
			{
				wafflesAndPathMovers->addObject(cell);
			}
			if (tile->getType() == "LiquidDrainerMatchObject" || tile->getType() == "LiquidFillerMatchObject")
			{
				liquids->addObject(cell);
			}
			if(cell->containsThoppler())
			{
				thopplers->addObject(cell);
			}
		}
	}

	specialTiles->setObject(breakers, BREAKERS);
	specialTiles->setObject(wafflesAndPathMovers, WAFFLEPATH);
	specialTiles->setObject(liquids, LIQUIDS);
	specialTiles->setObject(liquids, THOPPLERS);
	return specialTiles;
}

char BoardModel::getPathMoversCount()
{
	char count = 0;
	for (char i = 0; i < height; i++)
	{
		for (char j = 0; j < width; j++)
		{
			if (cells[i][j] == nullptr || cells[i][j]->isEmpty) continue;

			auto cell = cells[i][j];
			auto tile = cell->getSourceTile();
			if (tile == nullptr) continue;
			if(tile->getMovingTileType() == +MovingTileTypes::PathMoverMatchObject)
			{
				count++;
			}
		}
	}
	return count;
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
			auto cell = cells[i][j];
			if(i < currentLiquidLevel && !liquidSystem->containsIgnorePos(j, i) && !liquidSystem->containsIgnoreColumn(j) && !liquidSystem->containsIgnoreRow(i))
			{
				cell->inWater = true;
				cell->fallDirection = Direction::N;
				if (cell->containsPortalIn()) 
				{
					auto portalInTile = cell->getPortalIn();
					portalInTile->setInWater();
				}
				if (cell->containsPortalOut())
				{
					auto portalOutTile = cell->getPortalOut();
					portalOutTile->setInWater();
				}
			}
			else
			{
				cell->inWater = false;
				cell->fallDirection = Direction::S;
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

	if (pieceSwappers != nullptr)
	{
		for(auto swapper : *pieceSwappers)
		{
			CC_SAFE_DELETE(swapper.second);
		}
		CC_SAFE_DELETE(pieceSwappers);
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

std::map<std::string, CustomSpawnTableItem>* BoardModel::CreateCustomSpawnTablesListFromJson(rapidjson::Value& json)
{
	if (json.IsArray() && json.Size() > 0)
	{
		auto ret = new std::map<std::string, CustomSpawnTableItem>();
		auto arr = json.GetArray();

		for (auto& conv : arr)
		{
			CustomSpawnTableItem spt;
			spt.initWithJson(conv);
			ret->insert(std::pair<std::string, CustomSpawnTableItem>(spt.getName(), spt));
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
		if (liquidSystem == nullptr || liquidSystem->FillerToggle == 0)
		{
			for (char i = height - 1; i >= 0; i--)
			{
				auto cell = cells[i][j];
				if (!cell->isOutCell)
				{
					if (!cell->containsSpawner() && !cell->inWater)
					{
						auto spawner = SpawnerObject::create();
						spawner->initSpawner();
						cell->setTileToLayer(spawner, LayerId::Spawner);
						SpawnController::getInstance()->addSpawner(spawner);
					}
					break;
				}
			}
		}
		if(liquidSystem != nullptr && liquidSystem->DrainerToggle == 0)
		{
			for (char i = 0; i < height; i++)
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
						SpawnController::getInstance()->addSpawner(spawner);
					}
					break;
				}
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

	SpawnController::getInstance()->setCustomSpawnTable(CreateCustomSpawnTablesListFromJson(json["custom_spawn_table"]));
	CreateSpawnTableFromJson(json["spawn_table"]);

	auto liquidSpawnTable = CustomSpawnTableItem::CreateSpawnTablesFromJson(json["liquid_spawn_table"]);
	SpawnController::getInstance()->setLiquidSpawnTable(liquidSpawnTable);


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
				else if(strcmp(dataType, "ConveyorBeltObject") == 0)
				{
					if(conveyorInfo == nullptr)
					{
						conveyorInfo = new std::list<ConveyorInfo*>;
					}

					auto conveyorInfoItem = new ConveyorInfo;
					conveyorInfoItem->FromColumn = customData["from_column"].GetInt();
					conveyorInfoItem->FromRow = customData["from_row"].GetInt();
					conveyorInfoItem->ToColumn = customData["to_column"].GetInt();
					conveyorInfoItem->ToRow = customData["to_row"].GetInt();
					conveyorInfo->push_back(conveyorInfoItem);
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

ConveyorInfo* BoardModel::findConveyorInfo(Cell* cell) const
{
	if (cell == nullptr) return nullptr;
	if (conveyorInfo == nullptr || conveyorInfo->size() == 0) return nullptr;

	for(auto conveyorInfoItem : *conveyorInfo)
	{
		if(conveyorInfoItem->FromColumn == cell->gridPos.Col && conveyorInfoItem->FromRow == cell->gridPos.Row)
		{
			return conveyorInfoItem;
		}
	}
	return nullptr;
}

void BoardModel::buildConveyors()
{
	if(conveyors == nullptr)
	{
		conveyors = new std::list<CellsList*>;
	}
	if(conveyorInfo != nullptr)
	{
		for(auto conveyorInfoItem : *conveyorInfo)
		{
			auto conveyor = new CellsList;
			auto startCell = getCell(conveyorInfoItem->ToColumn, conveyorInfoItem->ToRow);
			auto endCell = getCell(conveyorInfoItem->FromColumn, conveyorInfoItem->FromRow);
			if (containedInConveyors(startCell)) continue;
			auto itrCell = startCell;
			auto prevCell = startCell;
			do
			{
				auto conveyorTile = static_cast<ConveyorBeltObject*>(itrCell->getTileAtLayer(LayerId::PathConveyor));
				if(conveyorTile == nullptr)
				{
					break;
				}
				conveyor->push_back(itrCell);
				
				auto strDir = conveyorTile->getDirectionString();
				auto indent = MoveDirectionsMap.at(strDir);
				prevCell = itrCell;
				itrCell = getCell(itrCell->gridPos.Col + indent.at(1), itrCell->gridPos.Row + indent.at(0));
				if(itrCell == nullptr || itrCell->isOutCell)
				{
					auto nextSectionInfo = findConveyorInfo(prevCell);
					if(nextSectionInfo != nullptr)
					{
						itrCell = getCell(nextSectionInfo->ToColumn, nextSectionInfo->ToRow);
					}
					else
					{
						break;
					}
				}
			}while (itrCell != startCell);
			if(conveyor->size() > 1)
			{
				conveyors->push_back(conveyor);
			}
			else
			{
				CC_SAFE_DELETE(conveyor);
			}
		}
	}

	for(char i = 0; i < height; i++)
	{
		for(char j = 0; j < width; j++)
		{
			auto cell = getCell(j, i);
			if(cell == nullptr) continue;

			auto startConveyor = static_cast<ConveyorBeltObject*>(cell->getTileAtLayer(LayerId::PathConveyor));
			if(startConveyor == nullptr || startConveyor->getType() != "ConveyorBeltObject") continue;
			if(containedInConveyors(cell)) continue;
			
			auto conveyor = new CellsList;
			auto itrCell = cell;
			do
			{
				auto conveyorTile = static_cast<ConveyorBeltObject*>(itrCell->getTileAtLayer(LayerId::PathConveyor));
				if (conveyorTile == nullptr)
				{
					break;
				}
				conveyor->push_back(itrCell);

				auto strDir = conveyorTile->getDirectionString();
				auto indent = MoveDirectionsMap.at(strDir);
				itrCell = getCell(itrCell->gridPos.Col + indent.at(1), itrCell->gridPos.Row + indent.at(0));
			}while (itrCell != nullptr && itrCell != cell);
			conveyors->push_back(conveyor);
		}
	}
}

bool BoardModel::containedInConveyors(Cell* cell) const
{
	if(conveyors == nullptr)
	{
		return false;
	}
	for(auto conveyor : *conveyors)
	{
		auto itr = std::find(conveyor->begin(), conveyor->end(), cell);
		if(itr != conveyor->end())
		{
			return true;
		}
	}
	return false;
}

bool BoardModel::checkConveyorStatus(CellsList* conveyor) const
{
	if(conveyor == nullptr || conveyor->size() == 0)
	{
		return false;
	}
	for(auto cell : *conveyor)
	{
		if (cell->isFixed) return false;

	}
	return true;
}

void BoardModel::moveConveyors()
{
	if (conveyors == nullptr || conveyors->size() == 0) return;

	for(auto conveyor : *conveyors)
	{
		if(!checkConveyorStatus(conveyor)) continue;

		auto cell = conveyor->back();
		auto lastTile = cell->getMovingTile();

		for(auto itr = conveyor->rbegin(); itr != --conveyor->rend(); ++itr)
		{
			auto next = std::next(itr);
			auto fromCell = *next;
			conveyTile(fromCell->getMovingTile(), *itr);
			fromCell->clear();
		}

		conveyTile(lastTile, conveyor->front());

	}
}

void BoardModel::conveyTile(MovingTile* from, Cell* to) const
{
	if (from == nullptr || to == nullptr) return;

	auto colIndent = abs(from->gridPos.Col - to->gridPos.Col);
	auto rowIndent = abs(from->gridPos.Row - to->gridPos.Row);
	if (colIndent + rowIndent > 1)
	{
		auto conveyorTile = to->getTileAtLayer(LayerId::PathConveyor);
		auto strDir = conveyorTile->getDirectionString();
		auto indent = MoveDirectionsMap.at(strDir);
		auto pos = to->getBoardPos() + Vec2(-indent.at(1) * CellSize, -indent.at(0) * CellSize);
		from->setPosition(pos);
	}

	from->showMoveAction(to);
	to->setSourceTile(from);
}

CellsList* BoardModel::getLavaCakeTargets()
{
	if(lavaCakeTargets == nullptr || lavaCakeTargets->size() == 0)
	{
		auto ret = new CellsList;
		for (char i = 0; i < 4; i++) 
		{
			auto target = getRandomCell();
			while (!target->canMove() || std::find(ret->begin(), ret->end(), target) != ret->end())
			{
				target = getRandomCell();
			}
			ret->push_back(target);
		}
		return ret;
	}
	else
	{
		auto retList = std::vector<Cell*>(lavaCakeTargets->begin(), lavaCakeTargets->end());
		while(retList.size() > 4)
		{
			retList.erase(retList.begin() + rand_0_1() * retList.size());
		}
		auto lst = new CellsList;
		std::copy(retList.begin(), retList.end(), std::back_inserter(*lst));
		return lst;
	}
}

void BoardModel::setIncreaseLavaCakeFlag(bool flag)
{
	if(lavaCakeTiles == nullptr || lavaCakeTiles->size() == 0)
	{
		return;
	}
	for(auto lava : *lavaCakeTiles)
	{
		lava->setIncreaseLavaCakeFlag(true);
	}
}

void BoardModel::addLavaCakeTile(LavaCakeObject* lavaCake)
{
	if(lavaCakeTiles == nullptr)
	{
		lavaCakeTiles = new std::list<LavaCakeObject*>;
	}
	lavaCakeTiles->push_back(lavaCake);
}

void BoardModel::addObjectSpinnerCell(Cell* cell)
{
	if(objectSpinnerCells == nullptr)
	{
		objectSpinnerCells = new CellsList;
	}
	objectSpinnerCells->push_back(cell);
}

void BoardModel::runObjectSpinner()
{
	if(objectSpinnerCells == nullptr || objectSpinnerCells->size() == 0)
	{
		return;
	}

	for(auto cell : *objectSpinnerCells)
	{
		auto spinner = cell->getTileAtLayer(LayerId::Waffle);
		rotateSpinner(cell, spinner->getDirectionString() == "clockwise");
	}
}

void BoardModel::rotateSpinner(Cell* cell, bool isClockWise)
{
	if(cell->isFixed || cell->rightCell->isFixed || cell->rightCell->downCell->isFixed || cell->downCell->isFixed)
	{
		return;
	}
	auto firstTile = cell->getMovingTile();
	if(isClockWise)
	{
		moveTile(cell, cell->downCell->getMovingTile());
		moveTile(cell->downCell, cell->downCell->rightCell->getMovingTile());
		moveTile(cell->downCell->rightCell, cell->rightCell->getMovingTile());
		moveTile(cell->rightCell, firstTile);
	}
	else
	{
		moveTile(cell, cell->rightCell->getMovingTile());
		moveTile(cell->rightCell, cell->rightCell->downCell->getMovingTile());
		moveTile(cell->rightCell->downCell, cell->downCell->getMovingTile());
		moveTile(cell->downCell, firstTile);
	}
}

MovingTile* BoardModel::moveTile(Cell* cell, MovingTile* movingTile)
{
	auto oldTile = cell->getMovingTile();
	if (movingTile != nullptr)
	{
		movingTile->showMoveAction(cell);
		cell->setSourceTile(movingTile);
	}
	else
	{
		cell->clear();
	}
	return oldTile;
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
	int n = layerNumber._to_integral();
	for (auto itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			auto gridPos = Utils::StrToGridPos(itr->name.GetString(), "_");
			assert(this->width > gridPos.Col && this->height > gridPos.Row);

			auto cell = cells[gridPos.Row][gridPos.Col];

			const auto itr1 = itr->value.FindMember("type");
			if (itr1 != itr->value.MemberEnd() && itr1->value.IsString())
			{
				const auto typeName = itr1->value.GetString();
				auto tile = PoolController::getInstance()->getCookieTile(typeName);
				if (tile != nullptr)
				{
					cell->isOutCell = false;
					tile->initWithGrid(gridPos.Col, gridPos.Row);
					tile->initWithJson(itr->value);

					cell->setTileToLayer(tile, layerNumber);

					if(strcmp(typeName, SEEKERPRIORITYOBJECT) == 0)
					{
						if(seekerPriorityList == nullptr)
						{
							seekerPriorityList = new std::list<SeekerPriorityObject*>;
						}
						seekerPriorityList->push_back(static_cast<SeekerPriorityObject*>(tile));
					}
					else if (strcmp(typeName, EMPTYOBJECT) == 0)
					{
						cell->isOutCell = true;
					}
					else if (strcmp(typeName, INVISIBLEBRICKOBJECT) == 0)
					{
						cell->isOutCell = true;
					}
					else if(strcmp(typeName, PORTALOUTLETOBJECT) == 0)
					{
						if(portalOutList == nullptr)
						{
							portalOutList = new std::list<PortalOutletObject*>;
						}
						portalOutList->push_back(reinterpret_cast<PortalOutletObject* const&>(tile));
					}
					else if (strcmp(typeName, PORTALINLETOBJECT) == 0)
					{
						if (portalInList == nullptr)
						{
							portalInList = new std::list<PortalInletObject*>;
						}
						portalInList->push_back(reinterpret_cast<PortalInletObject* const&>(tile));
					}
					else if (strcmp(typeName, LAVACAKETARGETOBJECT) == 0)
					{
						if (lavaCakeTargets == nullptr)
						{
							lavaCakeTargets = new CellsList;
						}
						lavaCakeTargets->push_back(cell);
					}
					else if(strcmp(typeName, SPAWNEROBJECT) == 0)
					{
						SpawnController::getInstance()->addSpawner(static_cast<SpawnerObject*>(tile));
					}
				}
			}
		}
	}

}

CellsList* BoardModel::findAvailableMoveCells()
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
					return new CellsList{
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
					availableMove = new CellsList{
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
			CellsList availableTiles;
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
					availableMove = new CellsList(availableTiles);
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