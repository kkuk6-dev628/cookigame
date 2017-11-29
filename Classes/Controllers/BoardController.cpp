#include "BoardController.h"
#include "Models/BoardModels/BoardModel.h"
#include "LevelController.h"
#include "Models/BoardModels/Cell.h"
#include "proj.win32/Macros.h"
#include "General/Constants.h"
#include "General/Utils.h"
#include "Models/Tiles/Tile.h"
#include "SpawnController.h"
#include "ActionController.h"
#include "Models/Tiles/MovingTile.h"
#include "Models/BoardModels/Match.h"

char BoardController::cellSize = 79;

BoardController::BoardController()
{
	currentLevel = LevelController::getInstance()->getCurrentLevel();
	spawnController = SpawnController::getInstance();
	actionController = ActionController::getInstance();
	poolController = PoolController::getInstance();

	pendingCrushCells = __Array::create();
	pendingCrushCells->retain();
}


BoardController::~BoardController()
{
	pendingCrushCells->release();
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
	if (cell == nullptr || cell->IsEmpty)
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
	if (targetCell == nullptr || targetCell->IsEmpty)
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
			targetTile->showSwapAction(selectedTile->gridPos, [this, matchIdTarget](){
				this->releaseWaitingMatch(matchIdTarget);
				cocos2d::log("matchid: %d", matchIdTarget);
			});
			swapTilesInternal(selectedTile->getCell(), targetCell);
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
		}
	}
	selectedTile = nullptr;
}


void BoardController::onTouchEnded(Touch* touch, Event* unused_event)
{
	if (selectedTile == nullptr)
	{
		return;
	}
	auto pos = convertToNodeSpace(touch->getLocation());
	const auto cell = getMatchCell(pos);
	if (cell == nullptr || cell->IsEmpty)
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
	if (gridPos.Col < 0 || gridPos.Col >= boardModel->Width 
		|| gridPos.Row < 0 || gridPos.Row >= boardModel->Height)
	{
		return nullptr;
	}
	return boardModel->getCell(gridPos.Col, gridPos.Row);
}

Cell* BoardController::getMatchCell(GridPos& pos) const
{
	if (pos.Col < 0 || pos.Col >= boardModel->Width || pos.Row < 0 || pos.Row >= boardModel->Height)
	{
		return nullptr;
	}
	return boardModel->getCell(pos.Col, pos.Row);
}

Cell* BoardController::getMatchCell(char col, char row) const
{
	if (col < 0 || col >= boardModel->Width || row < 0 || row >= boardModel->Height)
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
	backgroundLayer->initWithGrid(boardModel->Width, boardModel->Height);

	borderLayer = BoardLayer::create();
	borderLayer->initWithGrid(boardModel->Width, boardModel->Height);

	layeredMatchLayer = BoardLayer::create();
	layeredMatchLayer->initWithGrid(boardModel->Width, boardModel->Height);

	for (char i = 0; i < boardModel->Height; i++)
	{
		for (char j = 0; j < boardModel->Width; j++)
		{
			const auto cell = boardModel->getCell(j, i);

			if (cell->IsEmpty) continue;

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
		if (0 <= adjCol && adjCol < boardModel->Width
			&& 0 <= adjRow && adjRow < boardModel->Height
			&& !(boardModel->getCell(adjCol, adjRow)->IsEmpty))
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
	auto const width = CellSize * boardModel->Width;
	auto const height = CellSize * boardModel->Height;
	setContentSize(Size(width, height));
	const auto originX = CenterX - width / 2;
	const auto originY = CenterY - height / 2;
	setPosition(originX, originY);
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

Match* BoardController::findMatch(Cell* startCell)
{
	const auto color = startCell->getSourceTile()->Color;
	char left = 0, right = 0, up = 0, down = 0;
	// up check
	auto cell = startCell->upCell;
	CookieTile* tile = nullptr;
	while (cell != nullptr)
	{
		tile = cell->getSourceTile();
		if (cell->IsEmpty || tile == nullptr || color != tile->Color)
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
		if (cell->IsEmpty || tile == nullptr || color != tile->Color)
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
		if (cell->IsEmpty || tile == nullptr || color != tile->Color)
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
		if (cell->IsEmpty || tile == nullptr || color != tile->Color)
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
			if (cell == nullptr || cell->IsEmpty || cell->getSourceTile() == nullptr || color != cell->getSourceTile()->Color)
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
			cell = boardModel->getTurnCell(LayerId::Match, cell->gridPos, inDir, &newDir, false);
			inDir = newDir;
		}
	}
	if(match != nullptr) match->retain();
	return match;
}

void BoardController::swapTilesInternal(Cell* selectedCell, Cell* targetCell)
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
			if (match->getHMatchCount() > 2)
			{
				for (auto& cell : *(match->hMatchedCells))
				{
					if (cell == nullptr || cell->IsEmpty || cell->getSourceTile() == nullptr) continue;
					poolController->recycleCookieTile(cell->getSourceTile());
				}
			}
			if (match->getVMatchCount() > 2)
			{
				for (auto& cell : *(match->vMatchedCells))
				{
					if (cell == nullptr || cell->IsEmpty || cell->getSourceTile() == nullptr) continue;
					poolController->recycleCookieTile(cell->getSourceTile());
				}
			}
			if (match->getSMatchCount() > 3)
			{
				for (auto& cell : *(match->sMatchedCells))
				{
					if (cell == nullptr || cell->IsEmpty || cell->getSourceTile() == nullptr) continue;
					poolController->recycleCookieTile(cell->getSourceTile());
				}
			}
		}
	}

}

void BoardController::fallTiles()
{
	
}

void BoardController::crushCell(Cell* cell)
{
	if (cell == nullptr || cell->IsEmpty || cell->getSourceTile() == nullptr)
	{
		return;
	}
	poolController->recycleCookieTile(cell->getSourceTile());
	cell->clear();
}

void BoardController::update(float delta)
{
	Layer::update(delta);

	actionController->runPendingActions();
	crushPendingCells();
}
