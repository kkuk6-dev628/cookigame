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
#include <complex>

char BoardController::cellSize = 79;
GameState BoardController::gameState;

BoardController::BoardController()
{
	currentLevel = LevelController::getInstance()->getCurrentLevel();
	spawnController = SpawnController::getInstance();
	actionController = ActionController::getInstance();
	poolController = PoolController::getInstance();

	pendingCrushCells = __Array::create();
	pendingCrushCells->retain();
	pendingSeekers = __Array::create();
	pendingSeekers->retain();
	fallingTileCount = 0;
	gameState = GameState::Idle;
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
	processPendingSeekers();
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
	if (cell == nullptr || cell->isEmpty || fallingTileCount > 0 || gameState != GameState::Idle)
	{
		selectedTile = nullptr;
		return false;
	}

	auto movingTile = static_cast<MovingTile*>(cell->getSourceTile());
	if (movingTile != nullptr && movingTile->isMovable() && movingTile->isVisible())
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
	if (selectedTile == nullptr || fallingTileCount > 0 || gameState != GameState::Idle)
	{
		return;
	}
	auto pos = convertToNodeSpace(touch->getLocation());
	const auto offset = touch->getLocation() - touch->getStartLocation();

	if (offset.length() <= CellSize / 2) return;

	const auto dir = Utils::getDirection(offset);
	auto adjacentPos = Utils::getAdjacentPos(selectedTile->gridPos, dir);

	if (Utils::IsSameGrid(selectedTile->gridPos, adjacentPos)) 
	{
		return;
	}

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
			BoardController::gameState = SwappingTile;
			selectedTile->showSwapAction(targetCell->gridPos, [this, matchIdSelected](){
				this->releaseWaitingMatch(matchIdSelected);
			});
			targetTile->showSwapAction(selectedTile->gridPos, [this, matchIdTarget, targetCell](){
				BoardController::gameState = Idle;
				this->releaseWaitingMatch(matchIdTarget);
				if(this->selectedTile == nullptr)
				{
					return;
				}
				this->swapTilesInternal(this->selectedTile->getCell(), targetCell);
				this->selectedTile = nullptr;
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
	if (selectedTile == nullptr || fallingTileCount > 0 || gameState != GameState::Idle)
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

void BoardController::initBoardLayers()
{
	if (layersDict != nullptr)
	{
		DictElement* elem = nullptr;
		CCDICT_FOREACH(layersDict, elem)
		{
			auto layer = elem->getObject();
			CC_SAFE_DELETE(layer);
		}
		layersDict->removeAllObjects();
		CC_SAFE_DELETE(layersDict);
	}

	layersDict = __Dictionary::create();
	layersDict->retain();

	for(LayerId layerId : LayerId::_values())
	{
		auto boardLayer = BoardLayer::create();
		boardLayer->initWithGrid(boardModel->getWidth(), boardModel->getHeight());
		layersDict->setObject(boardLayer, layerId);
		addChild(boardLayer, layerId);
	}
	backgroundLayer		= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Background));
	borderLayer			= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Border));
	underCoverLayer		= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::UnderCover));
	coverLayer			= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Cover));
	pathConveyerLayer	= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::PathConveyor));
	waffleLayer			= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Waffle));
	layeredMatchLayer	= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Match));
	toppllerLayer		= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Toppling));
	targetLayer			= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Target));
	portalLayer			= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::Portal));
	showObjectsLayer	= static_cast<BoardLayer*>(layersDict->objectForKey(LayerId::ShowLayer));
}

BoardLayer* BoardController::getBoardLayer(LayerId layerId)
{
	return static_cast<BoardLayer*>(layersDict->objectForKey(layerId));
}

void BoardController::initBoardElements()
{
	initBoardLayers();

	for (char i = 0; i < boardModel->getHeight(); i++)
	{
		for (char j = 0; j < boardModel->getWidth(); j++)
		{
			const auto cell = boardModel->getCell(j, i);
			cell->setBoardLayer(layeredMatchLayer);

			if (cell->isEmpty || cell->isOutCell) continue;

			addBackgroundTile(j, i);
			for (LayerId layerId : LayerId::_values())
			{
				auto cellLayer = cell->getTileAtLayer(layerId);
				if(cellLayer == nullptr) continue;
				if (layerId._to_integral() == LayerId::Background || layerId._to_integral() == LayerId::Border) continue;
				getBoardLayer(layerId)->addChild(cellLayer);
			}

		}
	}

	addTile(1, 1, MovingTileTypes::XBreakerObject, TileColors::blue);
	addTile(2, 1, MovingTileTypes::ColumnBreakerObject, TileColors::red);
}

void BoardController::addTile(char col, char row, MovingTileTypes type, TileColors tileColor)
{
	auto cell = boardModel->getCell(col, row);
	if (cell == nullptr) return;
	if (cell->isOutCell) return;
	if (!cell->isEmpty)
	{
		poolController->recycleCookieTile(cell->getSourceTile());
		auto tile = poolController->getCookieTile(type._to_string());
		if (tile != nullptr)
		{
			tile->initWithGrid(col, row);
			tile->initWithType(type._to_string(), tileColor);
			cell->setSourceTile(tile);
		}
	}
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
			&& !(boardModel->getCell(adjCol, adjRow)->isEmpty)
			&& !(boardModel->getCell(adjCol, adjRow)->isOutCell))
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
	if(fallingTileCount > 0 || gameState != Idle)
	{
		return;
	}
	for(char i = 0; i < boardModel->getHeight(); i++)
	{
		for(char j = 0; j < boardModel->getWidth(); j++)
		{
			auto cell = getMatchCell(j, i);
			
			if(cell == nullptr ) continue;

			cell->countDownReserveCount();

			if(cell->isEmpty)
			{
				continue;
			}

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
	auto selectedTileType = selectedCell->getSourceTile()->getMovingTileType();
	auto targetTileType = targetCell->getSourceTile()->getMovingTileType();

	if(Utils::IsBonusTile(selectedTileType) && Utils::IsBonusTile(targetTileType))
	{
		auto match = Match::create();
		match->refCell = targetCell;
		match->bonusMatchCell = selectedCell;
		match->color = selectedCell->getSourceTile()->getTileColor();
		match->matchType = BonusMatch;
		match->matchId = getMatchId();
		selectedCell->getSourceTile()->matchId = match->matchId;
		match->isWaiting = true;
		match->retain();
		pendingCrushCells->addObject(match);
		return true;
	}
	if(selectedTileType._to_integral() == MovingTileTypes::RainbowObject)
	{
		auto match = Match::create();
		match->refCell = targetCell;
		match->color = targetCell->getSourceTile()->getTileColor();
		match->matchType = RainbowMatch;
		match->matchId = getMatchId();
		selectedCell->getSourceTile()->matchId = match->matchId;
		match->isWaiting = true;
		match->retain();
		pendingCrushCells->addObject(match);
		return true;
	}
	if (targetTileType._to_integral() == MovingTileTypes::RainbowObject)
	{
		auto match = Match::create();
		match->refCell = selectedCell;
		match->color = selectedCell->getSourceTile()->getTileColor();
		match->matchType = RainbowMatch;
		match->matchId = getMatchId();
		selectedCell->getSourceTile()->matchId = match->matchId;
		match->isWaiting = true;
		match->retain();
		pendingCrushCells->addObject(match);
		return true;
	}

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

Match* BoardController::findMatch(Cell* startCell)
{
	if(startCell->isEmpty || startCell->isOutCell || !startCell->getSourceTile()->canMatch)
	{
		return nullptr;
	}
	const auto color = startCell->getSourceTile()->getTileColor();
	char left = 0, right = 0, up = 0, down = 0;
	// up check
	auto cell = startCell->upCell;
	CookieTile* tile = nullptr;
	while (cell != nullptr)
	{
		tile = cell->getSourceTile();
		if (cell->isEmpty || cell->isOutCell || tile == nullptr || !tile->canMatch || color != tile->getTileColor())
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
		if (cell->isEmpty || cell->isOutCell || tile == nullptr || !tile->canMatch || color != tile->getTileColor())
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
		if (cell->isEmpty || cell->isOutCell || tile == nullptr || !tile->canMatch || color != tile->getTileColor())
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
		if (cell->isEmpty || cell->isOutCell || tile == nullptr || !tile->canMatch || color != tile->getTileColor())
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
		match->color = color;
		if (up + down >= 2)
		{
			for (int i = startCell->gridPos.Row - down; i <= startCell->gridPos.Row + up; i++)
			{
				setDirtyCell(startCell->gridPos.Col, i);
				match->vMatchedCells->push_back(getMatchCell(startCell->gridPos.Col, i));
			}
			match->up = up;
			match->down = down;
		}
		if (left + right >= 2)
		{
			for (int i = startCell->gridPos.Col - left; i <= startCell->gridPos.Col + right; i++)
			{
				setDirtyCell(i, startCell->gridPos.Row);
				match->hMatchedCells->push_back(getMatchCell(i, startCell->gridPos.Row));
			}
			match->left = left;
			match->right = right;
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
			if (cell == nullptr || cell->isEmpty || cell->isOutCell)
			{
				isSquareMatch = false;
				break;
			}
			else
			{
				tile = cell->getSourceTile();
				if(tile == nullptr || !tile->canMatch || color != tile->getTileColor())
				{
					isSquareMatch = false;
					break;
				}
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
		if (match == nullptr)
		{
			match = Match::create();
			match->color = color;
			match->refCell = startCell;
		}

		AdjacentDirs inDir = squareMatchedDir;
		AdjacentDirs newDir = AdjacentDirs::NoDir;
		cell = boardModel->getTurnCell(LayerId::Match, startCell->gridPos, squareMatchedDir, &newDir, false);
		for (auto j = 0; j < 3 && cell != nullptr; j++)
		{
			cell->dirty = true;
			match->sMatchedCells->push_back(cell);
			inDir = newDir;
			cell = boardModel->getTurnCell(LayerId::Match, cell->gridPos, inDir, &newDir, false);
		}
	}
	if(match != nullptr) match->retain();
	return match;
}

void BoardController::setDirtyCell(char col, char row)
{
	auto cell = getMatchCell(col, row);
	cell->dirty = true;
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

void BoardController::crushNormalMatch(Match* match)
{
	auto bonusType = match->getAvailableBonusType();
	if (match->refCell->getSourceTile() == nullptr)
	{
		if (bonusType._to_integral() != MovingTileTypes::LayeredMatchObject && !match->checkBonusCreated(bonusType))
		{
			auto bonusTile = poolController->getCookieTile(bonusType._to_string());
			bonusTile->initWithType(bonusType._to_string(), match->color);
			match->refCell->setSourceTile(bonusTile);
			match->refCell->reserveCount = 1;
			cocos2d::log("The ref cell is null!");
		}
		return;
	}
	auto color = match->refCell->getSourceTile()->getTileColor();
	if (match->hMatchedCells->size() > 2)
	{
		for (auto& cell : *match->hMatchedCells) if (match->refCell != cell) crushCell(cell);
	}
	if (match->vMatchedCells->size() > 2)
	{
		for (auto& cell : *match->vMatchedCells) if (match->refCell != cell) crushCell(cell);
	}
	if (match->sMatchedCells->size() > 2)
	{
		for (auto& cell : *match->sMatchedCells) if (match->refCell != cell) crushCell(cell);
	}

	crushCell(match->refCell);

	if (bonusType._to_integral() != MovingTileTypes::LayeredMatchObject)
	{
		auto bonusTile = poolController->getCookieTile(bonusType._to_string());
		bonusTile->initWithType(bonusType._to_string(), color);
		match->refCell->setSourceTile(bonusTile);
		match->refCell->reserveCount = 1;
	}
}

void BoardController::crushBonusMatch(Match* match)
{
	auto refCellBonusType = match->refCell->getMovingTile()->getMovingTileType();
	auto targetCellBonusType = match->bonusMatchCell->getMovingTile()->getMovingTileType();

	//match->bonusMatchCell->dirty = true;

	switch(refCellBonusType)
	{
	case MovingTileTypes::XBreakerObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::XBreakerObject:
			dualXCrush(match->refCell);
			break;
		case MovingTileTypes::BombBreakerObject:
			dualXCrush(match->refCell);
			break;
		case MovingTileTypes::RainbowObject:
			break;
		case MovingTileTypes::SeekerObject:
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			dualXCrush(match->refCell);
			break;
		default:
			break;
		}
		break;
	case MovingTileTypes::BombBreakerObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::XBreakerObject:
			dualXCrush(match->refCell);
			break;
		case MovingTileTypes::BombBreakerObject:
			break;
		case MovingTileTypes::RainbowObject:
			break;
		case MovingTileTypes::SeekerObject:
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			break;
		default:
			break;
		}
		break;
	case MovingTileTypes::RainbowObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::XBreakerObject:
			break;
		case MovingTileTypes::BombBreakerObject:
			break;
		case MovingTileTypes::RainbowObject:
			break;
		case MovingTileTypes::SeekerObject:
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			break;
		default:
			break;
		}
		break;
	case MovingTileTypes::SeekerObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::XBreakerObject:
			break;
		case MovingTileTypes::BombBreakerObject:
			break;
		case MovingTileTypes::RainbowObject:
			break;
		case MovingTileTypes::SeekerObject:
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			break;
		default:
			break;
		}
		break;
	case MovingTileTypes::ColumnBreakerObject:
	case MovingTileTypes::RowBreakerObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::XBreakerObject:
			dualXCrush(match->refCell);
			break;
		case MovingTileTypes::BombBreakerObject:
			break;
		case MovingTileTypes::RainbowObject:
			break;
		case MovingTileTypes::SeekerObject:
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			crushColumnBreaker(match->refCell);
			crushRowBreaker(match->refCell);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void BoardController::dualXCrush(Cell* cell)
{
	crushColumnBreaker(cell);
	crushRowBreaker(cell);
	crushXBreaker(cell);

}

void BoardController::crushRainbowMatch(Match* match)
{
	match->refCell->crushCell();
	auto sameColorCells = boardModel->getSameColorCells(match->color);
	for(auto cell : *sameColorCells)
	{
		crushCell(cell);
	}
	CC_SAFE_DELETE(sameColorCells);
}

void BoardController::crushMatch(Match* match)
{
	if(match != nullptr)
	{
		switch(match->matchType)
		{
		case NormalMatch:
			crushNormalMatch(match);
			break;
		case BonusMatch:
			crushBonusMatch(match);
			break;
		case RainbowMatch:
			crushRainbowMatch(match);
			break;
		default:
			break;
		}
	}
}

void BoardController::fallTiles()
{
	if(fallingTileCount > 0 || gameState != Idle)
	{
		return;
	}
	for(char j = 0; j < boardModel->getWidth(); j++)
	{
		for(char i = boardModel->getCurrentLiquidLevel(); i < boardModel->getHeight(); i++)
		{
			auto cell = boardModel->getCell(j, i);
			if(cell->isEmpty && cell->isFillable && !cell->isOutCell)
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
	if(cell->containsSpawner())
	{
		fallPath->startCell = cell;
		return fallPath;
	}
	auto fallCell = targetCell->getFallCell();
	while(fallCell != nullptr && fallCell->isEmpty && !fallCell->containsSpawner())
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

void BoardController::crushCell(Cell* cell)
{
	if (cell == nullptr || cell->isEmpty || cell->getSourceTile() == nullptr)
	{
		return;
	}
	
		auto tile = cell->getSourceTile();
		auto tileType = tile->getMovingTileType();
		switch (tileType)
		{
		case MovingTileTypes::BombBreakerObject:
			crushBombBreaker(cell);
			break;
		case MovingTileTypes::RowBreakerObject:
			crushRowBreaker(cell);
			break;
		case MovingTileTypes::ColumnBreakerObject:
			crushColumnBreaker(cell);
			break;
		case MovingTileTypes::XBreakerObject:
			crushXBreaker(cell);
			break;
		case MovingTileTypes::RainbowObject:
			break;
		case MovingTileTypes::SeekerObject:
			crushSeeker(cell);
			break;
		default:
			break;
		}
	cell->crushCell();
}

void BoardController::crushBombBreaker(Cell* cell)
{
	cell->crushCell();
	for(char i = cell->gridPos.Row - 2; i <= cell->gridPos.Row + 2; i++)
	{
		for(char j = cell->gridPos.Col - 2; j <= cell->gridPos.Col + 2; j++)
		{
			auto bombCell = getMatchCell(j, i);
			if(bombCell == nullptr) continue;
			if(std::abs(i - cell->gridPos.Row) == 2 && std::abs(j - cell->gridPos.Col) == 2) continue;
			crushCell(bombCell);
		}
	}
}

void BoardController::crushRowBreaker(Cell* cell)
{
	auto leftCell = cell->leftCell;
	cell->crushCell();
	while (leftCell != nullptr)
	{
		crushCell(leftCell);
		leftCell = leftCell->leftCell;
	}

	auto rightCell = cell->rightCell;
	while (rightCell != nullptr)
	{
		crushCell(rightCell);
		rightCell = rightCell->rightCell;
	}
}

void BoardController::crushColumnBreaker(Cell* cell)
{
	auto upCell = cell->upCell;
	cell->crushCell();
	while (upCell != nullptr)
	{
		crushCell(upCell);
		upCell = upCell->upCell;
	}

	auto downCell = cell->downCell;
	while (downCell != nullptr)
	{
		crushCell(downCell);
		downCell = downCell->downCell;
	}
}

void BoardController::crushXBreaker(Cell* cell)
{
	auto col = cell->gridPos.Col, row = cell->gridPos.Row;
	cell->crushCell();

	for(char c = col - 1, r = row -1; (c >= 0 && r >= 0); c--, r--)
	{
		auto xCell = getMatchCell(c, r);
		if(xCell == nullptr) continue;
		crushCell(xCell);
	}
	for (char c = col + 1, r = row + 1; (c < boardModel->getWidth() && r < boardModel->getHeight()); c++, r++)
	{
		auto xCell = getMatchCell(c, r);
		if (xCell == nullptr) continue;
		crushCell(xCell);
	}
	for (char c = col - 1, r = row + 1; (c >= 0 && r < boardModel->getHeight()); c--, r++)
	{
		auto xCell = getMatchCell(c, r);
		if (xCell == nullptr) continue;
		crushCell(xCell);
	}
	for (char c = col + 1, r = row - 1; (c < boardModel->getWidth() && r >= 0); c++, r--)
	{
		auto xCell = getMatchCell(c, r);
		if (xCell == nullptr) continue;
		crushCell(xCell);
	}
}

void BoardController::crushSeeker(Cell* cell)
{
	auto randomPos = boardModel->getRandomBoardPosition();
	auto seekerShow = poolController->getSeekerShow(cell->getSourceTile()->getTileColor());
	showObjectsLayer->addChild(seekerShow);
	seekerShow->setPosition(cell->getBoardPos());
	CKAction ckAction;
	ckAction.node = static_cast<Node*>(seekerShow);
	ckAction.action = actionController->createSeekerPendingAction(ckAction.node, randomPos);
	actionController->pushAction(ckAction, false);
	pendingSeekers->addObject(seekerShow);
	cell->crushCell();
}

void BoardController::processPendingSeekers()
{
	if(fallingTileCount > 0 || gameState != GameState::Idle)
	{
		return;
	}
	std::list<Cell*> targetsList;
	Ref* itr = nullptr;
	CCARRAY_FOREACH(pendingSeekers, itr)
	{
		auto seekerShow = static_cast<AnimationShowObject*>(itr);
		auto targetCell = boardModel->getSeekerTarget();
		if(targetCell == nullptr)
		{
			targetCell = findSeekerTarget();
			while(Utils::containsCell(&targetsList, targetCell))
			{
				targetCell = findSeekerTarget();
			}
		}
		if(targetCell == nullptr) continue;
		if(targetCell->getSourceTile() == nullptr) continue;
		targetsList.push_back(targetCell);
		landingSeeker(seekerShow, targetCell);
	}
	pendingSeekers->removeAllObjects();
}

void BoardController::landingSeeker(AnimationShowObject* seekerShow, Cell* targetCell)
{
	CKAction ckAction;
	auto targetPos = Utils::Grid2BoardPos(targetCell->gridPos);
	ckAction.node = seekerShow;
	auto crushingCell = targetCell;
	auto recycleSeeker = seekerShow;
	ckAction.action = actionController->createSeekerLandingAction(ckAction.node, targetPos, [this, recycleSeeker, crushingCell]()
	{
		this->crushCell(crushingCell);
		poolController->recycleSeekerShow(recycleSeeker);
	});
	actionController->pushAction(ckAction, false);
}

Cell* BoardController::findSeekerTarget()
{
	auto specialTiles = boardModel->getSpecialTiles();
	auto breakers = static_cast<__Array*>(specialTiles->objectForKey("breakers"));
	auto wafflePath = static_cast<__Array*>(specialTiles->objectForKey("wafflePath"));
	auto liquids = static_cast<__Array*>(specialTiles->objectForKey("liquids"));

	if (liquids->count() > 0)
	{
		return static_cast<Cell*>(liquids->getRandomObject());
	}
	if (wafflePath->count() > 0)
	{
		return static_cast<Cell*>(wafflePath->getRandomObject());
	}
	if (breakers->count() > 0)
	{
		return static_cast<Cell*>(breakers->getRandomObject());
	}
	return boardModel->getRandomCell();
}

void BoardController::update(float delta)
{
	Layer::update(delta);

}
