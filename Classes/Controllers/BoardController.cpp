#include "BoardController.h"
#include "Models/BoardModels/BoardModel.h"
#include "LevelController.h"
#include "Models/BoardModels/Cell.h"
#include "General/Constants.h"
#include "General/Utils.h"
#include "Models/Tiles/Tile.h"
#include "SpawnController.h"
#include "ActionController.h"
#include "Models/Tiles/MovingTile.h"
#include "Models/BoardModels/Match.h"
#include "Models/BoardModels/FallPath.h"
#include "Models/Tiles/SpawnerObject.h"

char BoardController::cellSize = 79;

BoardController::BoardController()
{
	currentLevel = LevelController::getInstance()->getCurrentLevel();
	spawnController = SpawnController::getInstance();
	actionController = ActionController::getInstance();
	poolController = PoolController::getInstance();

	pendingCrushCells = __Array::create();
	pendingCrushCells->retain();

	schedule(schedule_selector(BoardController::processLogic));
}

BoardController::~BoardController()
{
	pendingCrushCells->release();
}

bool BoardController::init()
{
	if(!Layer::init())
	{
		return false;
	}
	return true;
}

void BoardController::processLogic(float)
{
	actionController->runPendingActions();
	crushPendingCells();
	fallTiles();
	checkMatchesInBoard();
}

void BoardController::initControllersWithBoard() const
{
	//spawnController->setColorTable(boardModel->Colors);
}

void BoardController::initWithJson(rapidjson::Value& data)
{
	if (this->boardModel != nullptr)
	{
		delete this->boardModel;
	}
	this->boardModel = new BoardModel();
	this->boardModel->initWithJson(data);

	initControllersWithBoard();
	initNode();
	initBoardElements();
}

bool BoardController::onTouchBegan(Touch* touch, Event* unused_event)
{
	const auto pos = convertToNodeSpace(touch->getLocation());
	const auto cell = getMatchCell(pos);
	if (cell == nullptr || cell->isEmpty)
	{
		return false;
	}

	auto movingTile = static_cast<MovingTile*>(cell->getSourceTile());
	if (movingTile != nullptr && movingTile->isMovable())
	{
		selectedTile = movingTile;
		return true;
	}
	else
	{
		selectedTile = nullptr;
		return false;
	}
}

void BoardController::onTouchMoved(Touch* touch, Event* unused_event)
{
	if (selectedTile == nullptr)
	{
		return;
	}
	auto pos = convertToNodeSpace(touch->getLocation());
	const auto offset = touch->getLocation() - touch->getStartLocation();

	if (offset.length() <= CellSize / 2) return;

	const auto dir = Utils::getDirection(offset);
	auto adjacentPos = Utils::getAdjacentPos(selectedTile->gridPos, dir);

	if (Utils::IsSameGrid(selectedTile->gridPos, adjacentPos)) return;

	auto targetCell = getMatchCell(adjacentPos);
	if (targetCell == nullptr || targetCell->isEmpty)
	{
		selectedTile->showDirectionalScaleAction(dir);

	}
	else
	{
		auto targetTile = static_cast<MovingTile*>(targetCell->getSourceTile());
		const auto canSwap = canSwapTiles(selectedTile->getCell(), targetCell);
		if (canSwap)
		{
			auto matchIdSelected = selectedTile->matchId;
			auto matchIdTarget = targetTile->matchId;
			selectedTile->showSwapAction(targetCell->gridPos, [this, matchIdSelected](){
				this->releaseWaitingMatch(matchIdSelected);
				cocos2d::log("matchid: %d", matchIdSelected);
			});
			targetTile->showSwapAction(selectedTile->gridPos, [this, matchIdTarget, targetCell](){
				this->releaseWaitingMatch(matchIdTarget);
				this->swapTilesInternal(this->selectedTile->getCell(), targetCell);
				this->selectedTile = nullptr;
				cocos2d::log("matchid: %d", matchIdTarget);
			});
			doSomethingPerMove();
		}
		else
		{
			if (targetTile == nullptr || !targetTile->isMovable())
			{
				selectedTile->showDirectionalScaleAction(dir);
			}
			else
			{
				selectedTile->showDirectionalScaleAction(dir);
				targetTile->showDirectionalScaleAction(Utils::inverseDir(dir));
			}
			selectedTile = nullptr;

		}
	}
}


void BoardController::onTouchEnded(Touch* touch, Event* unused_event)
{
	if (selectedTile == nullptr)
	{
		return;
	}
	auto pos = convertToNodeSpace(touch->getLocation());
	const auto cell = getMatchCell(pos);
	if (cell == nullptr || cell->isEmpty)
	{
		return;
	}

	if (Utils::IsSameGrid(selectedTile->gridPos, cell->gridPos))
	{
		selectedTile->showScaleBouncingAction();
	}
}

void BoardController::onTouchCancelled(Touch* touch, Event* unused_event)
{
}

void BoardController::releaseWaitingMatch(const int matchId) const
{
	Ref* itr = nullptr;
	CCARRAY_FOREACH(pendingCrushCells, itr)
	{
		const auto match = static_cast<Match*>(itr);
		if (match->matchId == matchId)
		{
			match->isWaiting = false;
		}
	}
}

Cell* BoardController::getMatchCell(const Vec2 boardPos) const
{
	const auto gridPos = Utils::Board2GridPos(boardPos);
	if (gridPos.Col < 0 || gridPos.Col >= boardModel->getWidth() 
		|| gridPos.Row < 0 || gridPos.Row >= boardModel->getHeight())
	{
		return nullptr;
	}
	return boardModel->getCell(gridPos.Col, gridPos.Row);
}

Cell* BoardController::getMatchCell(GridPos& pos) const
{
	if (pos.Col < 0 || pos.Col >= boardModel->getWidth() || pos.Row < 0 || pos.Row >= boardModel->getHeight())
	{
		return nullptr;
	}
	return boardModel->getCell(pos.Col, pos.Row);
}

Cell* BoardController::getMatchCell(char col, char row) const
{
	if (col < 0 || col >= boardModel->getWidth() || row < 0 || row >= boardModel->getHeight())
	{
		return nullptr;
	}
	return boardModel->getCell(col, row);
}

void BoardController::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	// touches
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(BoardController::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(BoardController::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(BoardController::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(BoardController::onTouchCancelled, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	//start
	scheduleUpdate();
}

void BoardController::initBoardElements()
{
	if (backgroundLayer != nullptr)
	{
		CC_SAFE_DELETE(backgroundLayer);
	}
	backgroundLayer = BoardLayer::create();
	backgroundLayer->initWithGrid(boardModel->getWidth(), boardModel->getHeight());

	borderLayer = BoardLayer::create();
	borderLayer->initWithGrid(boardModel->getWidth(), boardModel->getHeight());

	layeredMatchLayer = BoardLayer::create();
	layeredMatchLayer->initWithGrid(boardModel->getWidth(), boardModel->getHeight());

	for (char i = 0; i < boardModel->getHeight(); i++)
	{
		for (char j = 0; j < boardModel->getWidth(); j++)
		{
			const auto cell = boardModel->getCell(j, i);
			cell->setBoardLayer(layeredMatchLayer);

			if (cell->isEmpty) continue;

			layeredMatchLayer->addChild(reinterpret_cast<Node*>(cell->getSourceTile()));
			addBackgroundTile(j, i);
		}
	}
	addChild(backgroundLayer);
	addChild(borderLayer);
	addChild(layeredMatchLayer);
	//auto obj = this->boardLayers->objectForKey(LayerId::Match);
	//assert(obj != nullptr, "Match layer not found!");
	//auto matchLayer = static_cast<BoardLayerModel*>(obj);
	//assert(matchLayer != nullptr, "Match layer is not valid!");
}

void BoardController::addBackgroundTile(const char col, const char row) const
{
	bool borders[4] = { false, false, false, false };
	auto backgroundTile = TileBase::create();
	backgroundTile->initWithGrid(col, row);
	backgroundTile->initTexture("bg_tile.png");
	backgroundLayer->addChild(backgroundTile);

	auto hasBorder = false;
	for (char k = 0; k < 4; k++)
	{
		const char adjCol = col + AdjacentIndents[k][1];
		const char adjRow = row + AdjacentIndents[k][0];
		if (0 <= adjCol && adjCol < boardModel->getWidth()
			&& 0 <= adjRow && adjRow < boardModel->getHeight()
			&& !(boardModel->getCell(adjCol, adjRow)->isEmpty))
		{
			borders[k] = false;
		}
		else
		{
			borders[k] = true;
			hasBorder = true;
		}
	}

	if (hasBorder)
	{
		auto borderTile = BorderTile::create();
		borderTile->initWithGrid(col, row);
		borderTile->initBorder(borders);
		borderLayer->addChild(borderTile);
	}

}

void BoardController::initNode()
{
	auto const width = CellSize * boardModel->getWidth();
	auto const height = CellSize * boardModel->getHeight();
	setContentSize(Size(width, height));
	const auto originX = CenterX - width / 2;
	const auto originY = CenterY - height / 2;
	setPosition(originX, originY);
}

void BoardController::checkMatchesInBoard()
{
	if(actionController->getRunningActionCount() > 0)
	{
		return;
	}
	for(char i = 0; i < boardModel->getHeight(); i++)
	{
		for(char j = 0; j < boardModel->getWidth(); j++)
		{
			auto cell = getMatchCell(j, i);
			
			if(cell == nullptr) continue;

			auto match = findMatch(cell);

			if(match == nullptr) continue;

			match->matchId = getMatchId();
			cell->getSourceTile()->matchId = match->matchId;
			match->isWaiting = false;
			pendingCrushCells->addObject(match);
		}
	}
}


int BoardController::canSwapTiles(Cell* selectedCell, Cell* targetCell, bool addToCrush)
{
	auto canSwap = false;
	swapTilesInternal(selectedCell, targetCell);
	{
		auto matchSelected = findMatch(selectedCell);
		auto matchTarget = findMatch(targetCell);
		if (matchSelected != nullptr)
		{
			canSwap = true;
			matchSelected->matchId = getMatchId();
			selectedCell->getSourceTile()->matchId = matchSelected->matchId;
			matchSelected->isWaiting = true;
			pendingCrushCells->addObject(matchSelected);
		}
		if (matchTarget != nullptr)
		{
			canSwap = true;
			matchTarget->matchId = getMatchId();
			targetCell->getSourceTile()->matchId = matchTarget->matchId;
			matchTarget->isWaiting = true;
			pendingCrushCells->addObject(matchTarget);
		}
	}
	swapTilesInternal(selectedCell, targetCell);
	return canSwap;
}

Match* BoardController::findMatch(Cell* startCell) const
{
	const auto color = startCell->getSourceTile()->getTileColor();
	char left = 0, right = 0, up = 0, down = 0;
	// up check
	auto cell = startCell->upCell;
	CookieTile* tile = nullptr;
	while (cell != nullptr)
	{
		tile = cell->getSourceTile();
		if (cell->isEmpty || tile == nullptr || color != tile->getTileColor())
		{
			break;
		}
		cell = cell->upCell;
		up++;
	}
	// down check
	cell = startCell->downCell;
	while (cell != nullptr)
	{
		tile = cell->getSourceTile();
		if (cell->isEmpty || tile == nullptr || color != tile->getTileColor())
		{
			break;
		}
		cell = cell->downCell;
		down++;
	}
	// left check
	cell = startCell->leftCell;
	while (cell != nullptr)
	{
		tile = cell->getSourceTile();
		if (cell->isEmpty || tile == nullptr || color != tile->getTileColor())
		{
			break;
		}
		cell = cell->leftCell;
		left++;
	}
	// right check
	cell = startCell->rightCell;
	while (cell != nullptr)
	{
		tile = cell->getSourceTile();
		if (cell->isEmpty || tile == nullptr || color != tile->getTileColor())
		{
			break;
		}
		cell = cell->rightCell;
		right++;
	}

	Match* match = nullptr;
	if (up + down >= 2 || left + right >= 2)
	{
		match = Match::create();
		match->refCell = startCell;
		if (up + down >= 2)
		{
			for (int i = startCell->gridPos.Row - down; i <= startCell->gridPos.Row + up; i++)
			{
				match->vMatchedCells->push_back(getMatchCell(startCell->gridPos.Col, i));
			}
		}
		if (left + right >= 2)
		{
			for (int i = startCell->gridPos.Col - left; i <= startCell->gridPos.Col + right; i++)
			{
				match->hMatchedCells->push_back(getMatchCell(i, startCell->gridPos.Row));
			}
		}
	}   

	AdjacentDirs squareMatchedDir = AdjacentDirs::NoDir;
	for (auto i = 0; i < 4; i++)
	{
		auto inDir = AdjacentDirs::_from_integral(i);
		auto isSquareMatch = true;
		AdjacentDirs newDir = AdjacentDirs::NoDir;
		cell = boardModel->getTurnCell(LayerId::Match, startCell->gridPos, inDir, &newDir, false);
		for (auto j = 0; j < 3; j++)
		{
			if (cell == nullptr || cell->isEmpty || cell->getSourceTile() == nullptr || color != cell->getSourceTile()->getTileColor())
			{
				isSquareMatch = false;
				break;
			}
			inDir = newDir;
			cell = boardModel->getTurnCell(LayerId::Match, cell->gridPos, inDir, &newDir, false);
		}
		if (isSquareMatch)
		{
			squareMatchedDir = AdjacentDirs::_from_integral(i);
			break;
		}
	}

	if (squareMatchedDir._to_integral() >= 0 && squareMatchedDir._to_integral() < 4)
	{
		if(match == nullptr) match = Match::create();

		match->refCell = startCell;
		AdjacentDirs inDir = squareMatchedDir;
		AdjacentDirs newDir = AdjacentDirs::NoDir;
		cell = boardModel->getTurnCell(LayerId::Match, startCell->gridPos, squareMatchedDir, &newDir, false);
		for (auto j = 0; j < 3 && cell != nullptr; j++)
		{
			match->sMatchedCells->push_back(cell);
			inDir = newDir;
			cell = boardModel->getTurnCell(LayerId::Match, cell->gridPos, inDir, &newDir, false);
		}
	}
	if(match != nullptr) match->retain();
	return match;
}

void BoardController::swapTilesInternal(Cell* selectedCell, Cell* targetCell) const
{
	const auto selectedTile = selectedCell->getSourceTile();
	selectedCell->setSourceTile(targetCell->getSourceTile());
	targetCell->setSourceTile(selectedTile);
}

void BoardController::doSomethingPerMove()
{
	moveCount++;
}

void BoardController::crushPendingCells()
{
	Ref* itr = nullptr;
	CCARRAY_FOREACH(pendingCrushCells, itr)
	{
		const auto match = static_cast<Match*>(itr);
		if (!match->isWaiting)
		{
			crushMatch(match);
			pendingCrushCells->fastRemoveObject(match);
		}
	}

}

void BoardController::crushMatch(Match* match)
{
	crushCell(match->refCell);
	if (match->hMatchedCells->size() > 2)
	{
		for (auto& cell : *match->hMatchedCells) crushCell(cell);
	}
	if (match->vMatchedCells->size() > 2)
	{
		for (auto& cell : *match->vMatchedCells) crushCell(cell);
	}
	if (match->sMatchedCells->size() > 2)
	{
		for (auto& cell : *match->sMatchedCells) crushCell(cell);
	}

}

void BoardController::fallTiles()
{
	for(char j = 0; j < boardModel->getWidth(); j++)
	{
		for(char i = boardModel->getCurrentLiquidLevel(); i < boardModel->getHeight(); i++)
		{
			auto cell = boardModel->getCell(j, i);
			if(cell->isEmpty && cell->isFillable)
			{
				auto fallPath = findFallPath(cell);
				if(fallPath->startCell != nullptr)
				{
					if(fallPath->startCell->isEmpty)
					{
						if(fallPath->startCell->containsSpawner())
						{
							fallPath->startCell->spawnMatchTile();
							if(Utils::IsSameGrid(fallPath->startCell->gridPos, cell->gridPos))
							{
								static_cast<SpawnerObject*>(cell->getTileAtLayer(LayerId::Spawner))->initSpawnedCount();
							}
						}
					}
					if (!fallPath->startCell->isEmpty)
					{
						fallPath->showFallAction();
						if(!Utils::IsSameGrid(fallPath->startCell->gridPos, cell->gridPos))
						{
							cell->setSourceTile(fallPath->startCell->getSourceTile());
							fallPath->startCell->clear();
						}
						cell->getSourceTile()->setCellPos();

					}
				}
			}
		}
	}
}

FallPath* BoardController::findFallPath(Cell* cell)
{
	auto fallPath = new FallPath;
	fallPath->endCell = cell;
	auto targetCell = cell;
	auto fallCell = targetCell->getFallCell();
	while(fallCell != nullptr && fallCell->isEmpty)
	{
		if(fallCell->gridPos.Col != targetCell->gridPos.Col)
		{
			fallPath->pushGridPos(targetCell->gridPos);
			fallPath->pushGridPos(fallCell->gridPos);
		}
		targetCell = fallCell;
		fallCell = targetCell->getFallCell();
	}
	if(fallCell != nullptr)
	{
		fallPath->startCell = fallCell;
	}
	else if(targetCell->containsSpawner())
	{
		fallPath->startCell = targetCell;
	}
	
	return fallPath;
}

void BoardController::crushCell(Cell* cell) const
{
	if (cell == nullptr || cell->isEmpty || cell->getSourceTile() == nullptr)
	{
		return;
	}
	
	cell->crushCell();
}

void BoardController::update(float delta)
{
	Layer::update(delta);

}
