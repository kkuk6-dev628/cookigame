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
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Ext/Math.h"
#include "Layers/Dialogs.h"
#include "Scenes/LevelMapScene.h"
#include "GameController.h"
#include "Models/Tiles/LavaCakeObject.h"
#include "Scenes/GamePlayScene.h"

char BoardController::cellSize = 79;
GameState BoardController::gameState;

BoardController::BoardController()
{
	currentLevel = LevelController::getInstance()->getCurrentLevel();
	spawnController = SpawnController::getInstance();
	actionController = ActionController::getInstance();
	poolController = PoolController::getInstance();
	scoreController = ScoreController::getInstance();
	soundController = SoundController::getInstance();

	pendingCrushCells = __Array::create();
	pendingCrushCells->retain();
	pendingSeekers = __Array::create();
	pendingSeekers->retain();
	fallingTileCount = 0;
	gameState = GameState::Idle;
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

void BoardController::processLogic(float dt)
{
	hintTime -= dt;
	if(hintTime <= 0)
	{
		showHintAction();
		hintTime = HINT_TIME;
	}
	actionController->runPendingActions();
	crushPendingCells();
	fallTilesLoop();
	checkMatchesInBoard();

	if (pendingCrushCells->count() > 0)
	{
		initHintAction();
		return;
	}
	processPendingSeekers();
	processCustomLogic(dt);
	moveConveyors();
	moveSpinners();
	moveSwappers();
	spreedHoneyModifier();
	spawnController->resetSpawners();

	checkObjective();
	checkMoveCount();

	scoreController->processScoreQueue();

	doShuffle();

	checkFallingTileCount();
}

void BoardController::checkFallingTileCount()
{
	if(fallingTileCount > 0)
	{
		checkFallingCount++;
		if(checkFallingCount > 300)
		{
			fallingTileCount = 0;
			checkFallingCount = 0;
		}
	}
}


void BoardController::initWithNode(Node* node, Node* effect)
{
	rootNode = node;
	topMenuArea = rootNode->getChildByName("top_menu_area");

	scoreController->setTotalScoreNode(topMenuArea);
	scoreController->setStarScores(currentLevel->getScores()->at(0), currentLevel->getScores()->at(1), currentLevel->getScores()->at(2));

	bottomMenuArea = rootNode->getChildByName("bottom_menu_area");
	moveCountNode = static_cast<ui::Text*>(topMenuArea->getChildByName("move_number"));
	updateMoveCountText();

	scoreTextNode = static_cast<ui::Text*>(topMenuArea->getChildByName("score"));
	objectCountNode = static_cast<ui::Text*>(topMenuArea->getChildByName("object_count"));
	effectNode = effect;

	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_6")->setVisible(true);

	objectTargetPos = Utils::convertPos(circleGroupNode, effectNode);
	for (char i = 1; i <= 6; i++)
	{
		auto nodeName = StringUtils::format("FileNode_%d", i);
		circleGroupNode->getChildByName(nodeName)->setVisible(false);
	}

	auto const width = CellSize * boardModel->getWidth();
	auto const height = CellSize * boardModel->getHeight();
	setContentSize(Size(width, height));
	const auto originX = CenterX - width / 2;
	const auto originY = CenterY - height / 2;
	centerPos.set(width / 2, height / 2);
	setPosition(originX, originY);
}


GoalTypes BoardController::getGoalType()
{
	if(boardModel != nullptr)
	{
		return boardModel->getGoals()->front().GoalType;
	}
	else
	{
		return GoalTypes::WaffleObject;
	}
}

void BoardController::initWithModel(BoardModel* model)
{
	if (this->boardModel != nullptr)
	{
		delete this->boardModel;
	}
	this->boardModel = model;

	initBoardElements();
	initAnimations();
	schedule(schedule_selector(BoardController::processLogic), 0.05);
}

void BoardController::initWithJson(rapidjson::Value& data)
{
	if (this->boardModel != nullptr)
	{
		delete this->boardModel;
	}
	this->boardModel = new BoardModel();
	this->boardModel->initWithJson(data);

	initBoardElements();
	schedule(schedule_selector(BoardController::processLogic), 0.1);
}

void BoardController::initAnimations()
{
	if(shuffleAnimation != nullptr)
	{
		CC_SAFE_DELETE(shuffleAnimation);
	}
	shuffleAnimation = AnimationShowObject::create();
	shuffleAnimation->initWithCSB("res/animation/box.csb");
	//rootNode->setContentSize(Size(360, 640));
	shuffleAnimation->setScale(0.5f);
	shuffleAnimation->setPosition(Vec2(-50, -320));
	showObjectsLayer->addChild(shuffleAnimation, 500);
	shuffleAnimation->setVisible(false);
}

void BoardController::showGameWinDlg()
{
	auto dlg = GameWinDialog::create();
	auto buttonText = static_cast<ui::Text*>(dlg->btn_next->getChildByName("text_1"));
	if(gameState == Failed)
	{

		buttonText->setString("Restart");
	}
	else
	{
		buttonText->setString("Next");
	}

	dlg->btn_close->addClickEventListener([this, dlg](Ref*) {
		//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
		dlg->close();
		this->endGame();
	});
	dlg->btn_next->addClickEventListener([this, dlg](Ref*) {
		//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
		dlg->close();
		if (gameState == Failed)
		{
			auto gamePlayScene = GamePlayScene::getInstance();
			gamePlayScene->restartGame();
		}
		else
		{
			Director::getInstance()->popScene();
			GameController::getInstance()->goGamePlay(currentLevel->getLevelNumber() + 1);
		}
		BoardController::gameState = Idle;
	});
	dlg->retain();
	dlg->show(this->getParent(), LayerId::ShowLayer);

}

void BoardController::showGameFailedDlg()
{
	auto dlg = GameFailedDialog::create();
	dlg->btn_close->addClickEventListener([this, dlg](Ref*) {
		//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
		dlg->close();
		this->endGame();
	});
	dlg->btn_playon->addClickEventListener([=](Ref*){
		dlg->close();
		moveCount -= 10;
		updateMoveCountText();

		gameState = Idle;
	});
	dlg->retain();
	dlg->show(this->getParent(), LayerId::ShowLayer);
}

void BoardController::increaseObjectCount()
{
	collectedObjectCount++;
	objectCountNode->setString(StringUtils::toString(totalObjectCount - collectedObjectCount));
}

void BoardController::checkObjective()
{
	if (fallingTileCount > 0 || gameState != Idle) return;
	if (totalObjectCount == collectedObjectCount)
	{
		finishLevel();
	}
}

void BoardController::checkMoveCount()
{
	if(gameState == Idle && currentLevel->getMoveCount() <= moveCount)
	{
		gameState = Failed;
		showGameFailedDlg();
	}
}

void BoardController::finishLevel()
{
	gameState = GameState::Completed;
	UserData::getInstance()->setTopLevel(currentLevel->getLevelNumber() + 1);
	showGameWinDlg();
}


void BoardController::endGame()
{
	BoardController::gameState = Idle;
	LevelMapScene::getInstance()->refresh(true);
	Director::getInstance()->popScene();
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
	if (movingTile != nullptr && !cell->isFixed && movingTile->isVisible())
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
	if (targetCell == nullptr || targetCell->isEmpty || targetCell->isFixed)
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
			soundController->playEffectSound(SoundEffects::sound_gem_drag);
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
			soundController->playEffectSound(SoundEffects::sound_gem_drag_fail);
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

	scoreController->setScoresContainerNode(showObjectsLayer);
}

BoardLayer* BoardController::getBoardLayer(LayerId layerId)
{
	return static_cast<BoardLayer*>(layersDict->objectForKey(layerId));
}

void BoardController::addMask()
{
	maskNode = ClippingNode::create();
	maskNode->setStencil(stencil);
	maskNode->setInverted(true);
	auto sprite = Sprite::create("images/background.png");
	//sprite->setSpriteFrame("Game_001_BG.png");
	addChild(maskNode, LayerId::Target);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	sprite->setAnchorPoint(Vec2(0.5, 0.5));
	maskNode->setPosition(0, 0);
	maskNode->setAnchorPoint(Vec2(0, 0));
	maskNode->addChild(sprite);

	auto x = boardModel->getWidth() * CellSize / 2;
	auto y = boardModel->getHeight() * CellSize / 2;
	sprite->setPosition(x, y);
}

void BoardController::initBoardElements()
{
	initBoardLayers();

	for (char i = 0; i < boardModel->getHeight(); i++)
	{
		for (char j = 0; j < boardModel->getWidth(); j++)
		{
			addCellToBoard(j, i);
		}
	}
	
	addMask();
	initLiquidLayer();
	boardModel->buildConveyors();

	//addTile(4, 3, MovingTileTypes::SeekerObject, TileColors::red);
	//addTile(5, 3, MovingTileTypes::RowBreakerObject, TileColors::red);
	//addTile(4, 4, MovingTileTypes::ColumnBreakerObject, TileColors::yellow);
}

void BoardController::addCellToBoard(char col, char row)
{
	const auto cell = boardModel->getCell(col, row);
	cell->setBoardLayer(layeredMatchLayer);

	addBackgroundTile(col, row);
	if (cell->isOutCell)
	{
		addTileToLiquidMask(cell->gridPos);
		return;
	}

	for (LayerId layerId : LayerId::_values())
	{
		auto cellLayer = cell->getTileAtLayer(layerId);
		if (cellLayer == nullptr) continue;
		if (layerId == +LayerId::Background || layerId == +LayerId::Border || cellLayer->getParent()!= nullptr) continue;

		if(cellLayer->getType() == PRETZELOBJECT)
		{
			getBoardLayer(layerId)->addChild(cellLayer, PRETZELZORDER);
		}
		else
		{
			getBoardLayer(layerId)->addChild(cellLayer);
		}
		if (cellLayer->getType() == OBJECTSPINNEROBJECT)
		{
			boardModel->addObjectSpinnerCell(cell);
		}
		else if(cellLayer->getType() == PIECESWAPPEROBJECT)
		{
			boardModel->addPieceSwapperCell(cellLayer->getTileColor()._to_string(), cell);
		}
	}

	auto tile = cell->getSourceTile();
	if(tile != nullptr)
	{
		if (tile->getType() == LAVACAKEOBJECT)
		{
			boardModel->addLavaCakeTile(static_cast<LavaCakeObject*>(tile));
			addLavaCakeObject(col + 1, row, tile);
			addLavaCakeObject(col + 1, row - 1, tile);
			addLavaCakeObject(col, row - 1, tile);
		}
	}

}

void BoardController::addLavaCakeObject(char col, char row, CookieTile* lavaCake)
{
	auto cell = getMatchCell(col, row);
	if (cell == nullptr || cell->isOutCell) return;
	auto oldTile = cell->getSourceTile();
	if(oldTile != nullptr && oldTile->getType() == POINTEROBJECT)
	{
		if (oldTile->getParent() != nullptr) oldTile->removeFromParent();
		cell->setSourceTile(lavaCake);
	}
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

void BoardController::addMovingTile(Cell* cell, MovingTileTypes type, TileColors tileColor)
{
	if (cell == nullptr) return;
	if (cell->isOutCell) return;
	if (!cell->isEmpty)
	{
		poolController->recycleCookieTile(cell->getSourceTile());
	}
	auto tile = poolController->getCookieTile(type._to_string());
	if (tile != nullptr)
	{
		tile->initWithGrid(cell->gridPos.Col, cell->gridPos.Row);
		tile->initWithType(type._to_string(), tileColor);
		cell->setSourceTile(tile);
	}
}

void BoardController::addTileToMask(char col, char row)
{
	if (stencil == nullptr) {
		stencil = DrawNode::create();
	}

	auto origin = Utils::Grid2BoardPos(col, row);
	origin.x -= CellSize / 2 + 1;
	origin.y -= CellSize / 2 + 1;
	auto dest = origin + Vec2(CellSize + 2, CellSize + 2);
	stencil->drawSolidRect(origin, dest, Color4F::GREEN);
}

void BoardController::addTileToLiquidMask(GridPos& gridPos)
{
	if (liquidStencil == nullptr)
	{
		liquidStencil = DrawNode::create();
	}
	auto origin = Utils::Grid2BoardPos(gridPos);
	origin.x -= CellSize / 2;
	origin.y -= CellSize / 2;
	auto dest = origin + Vec2(CellSize, CellSize);
	liquidStencil->drawSolidRect(origin, dest, Color4F::GREEN);
}

void BoardController::initLiquidLayer()
{
	auto liquidSys = boardModel->getLiquidSystem();
	if(liquidSys != nullptr)
	{
		if (liquidStencil == nullptr)
		{
			liquidStencil = DrawNode::create();
		}

		for(auto ignorePos : liquidSys->IgnoreGridPos)
		{
			addTileToLiquidMask(ignorePos);
		}

		for(auto ignorCol : liquidSys->IgnoreColumns)
		{
			auto origin = Utils::Grid2BoardPos(ignorCol, 0);
			origin -= Vec2(CellSize / 2, CellSize / 2);
			auto dest = origin + Vec2(CellSize, CellSize * boardModel->getHeight());
			liquidStencil->drawSolidRect(origin, dest, Color4F::GREEN);
		}
		for(auto ignoreRow:liquidSys->IgnoreRows)
		{
			auto origin = Utils::Grid2BoardPos(0, ignoreRow);
			origin -= Vec2(CellSize / 2, CellSize / 2);
			auto dest = origin + Vec2(CellSize * boardModel->getWidth(), CellSize);
			liquidStencil->drawSolidRect(origin, dest, Color4F::GREEN);
		}
		liquidMask = ClippingNode::create();
		liquidMask->setStencil(liquidStencil);
		liquidMask->setInverted(true);
		liquidNode = CSLoader::createNode("res/animation/water.csb");

		auto action = CSLoader::createTimeline("res/animation/water.csb");
		action->gotoFrameAndPlay(0, true);
		
		liquidNode->runAction(action);
		liquidNode->setPosition(0, liquidSys->LevelStart * CellSize);
		liquidMask->setPosition(0, 0);
		liquidMask->setAnchorPoint(Vec2(0, 0));
		liquidMask->addChild(liquidNode);
		addChild(liquidMask, LayerId::Waffle);

	}
	
}

void BoardController::showLineCrushEffect(Cell* cell, float rotation, char side)
{
	if (side > -1)
	{
		auto right = poolController->getLineCrushShow();
		right->setPosition(cell->getBoardPos());
		right->setRotation(rotation);
		showObjectsLayer->addChild(right);
	}

	if (side < 1) 
	{
		auto left = poolController->getLineCrushShow();
		left->setPosition(cell->getBoardPos());
		left->setRotation(rotation + 180);
		showObjectsLayer->addChild(left);
	}
}

void BoardController::toggleLiquidFiller(std::string fillerType)
{
	for (char i = 0; i < boardModel->getHeight(); i++)
	{
		for (char j = 0; j < boardModel->getWidth(); j++)
		{
			auto cell = getMatchCell(j, i);
			if (cell == nullptr || cell->isOutCell || cell->isEmpty)
			{
				continue;
			}
			auto fillerTile = cell->getMovingTile();
			if (fillerTile->getType() == fillerType)
			{
				fillerTile->initWithType((+MovingTileTypes::LayeredMatchObject)._to_string(), fillerTile->getTileColor());
			}
		}
	}
}

void BoardController::showBombAndLineCrushEffect(Cell* cell)
{
	auto effect = poolController->getBombAndLineCrushShow();
	effect->setPosition(cell->getBoardPos());
	showObjectsLayer->addChild(effect);
}

void BoardController::showBombCrushEffect(Cell* cell)
{
	auto effect = poolController->getBombCrushShow();
	effect->setPosition(cell->getBoardPos());
	showObjectsLayer->addChild(effect);
}

void BoardController::addBackgroundTile(const char col, const char row)
{
	auto cell = getMatchCell(col, row);
	auto tile = cell->getSourceTile();

	if(tile != nullptr && (strcmp(tile->getType().c_str(), EMPTYOBJECT) == 0 || strcmp(tile->getType().c_str(), INVISIBLEBRICKOBJECT) == 0))
	{
		cell->isOutCell = cell->isEmpty = true;
		return;
	}

	addTileToMask(col, row);

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
			if (findMatchInPendings(match) == nullptr) 
			{
				pendingCrushCells->addObject(match);
			}
			//else
			//{
			//	cocos2d::log("duplicated match: Col - %d, Row - %d", j, i);
			//}
		}
	}
}

Match* BoardController::findMatchInPendings(Match* newMatch)
{
	Ref* itr;
	CCARRAY_FOREACH(pendingCrushCells, itr)
	{
		auto match = static_cast<Match*>(itr);
		if(match->equal(newMatch))
		{
			return match;
		}
	}
	return nullptr;
}

void BoardController::doShuffle()
{
	if (fallingTileCount > 0 || gameState != Idle || pendingCrushCells->count() > 0)
	{
		return;
	}

	if(boardModel->isShuffleNeed())
	{
		gameState = GameState::Shuffling;
		showShuffleAction();
	}
}

void BoardController::moveConveyors()
{
	if (fallingTileCount > 0 || gameState != Idle || pendingCrushCells->count() > 0 || !moveConveyorsFlag)
	{
		return;
	}

	moveConveyorsFlag = false;
	boardModel->moveConveyors();
}

void BoardController::showHintAction()
{
	if (boardModel->availableMove == nullptr) return;
	auto color = TileColors::any;
	for (auto cell : *boardModel->availableMove)
	{
		auto tile = cell->getMovingTile();
		if (tile == nullptr)
		{
			return;
		}
		if (color == TileColors::any)
		{
			color = tile->getTileColor();
		}
		if (+color != tile->getTileColor())
		{
			return;
		}
	}
	for (auto cell : *boardModel->availableMove)
	{
		if (cell->getMovingTile() == nullptr || cell->isFixed)
		{
			continue;
		}
		auto hintAction = Repeat::create(
			Sequence::create(
				ScaleTo::create(0.3f, 0.7f),
				ScaleTo::create(0.3f, 1.0f),
				nullptr
			),
			5
		);

		hintAction->setTag(HINT_ACTION);
		cell->getMovingTile()->runAction(hintAction);
	}
}


void BoardController::manualShuffle()
{
	gameState = GameState::Shuffling;
	showShuffleAction();
}

void BoardController::shuffle(float)
{
	boardModel->shuffle(showObjectsLayer);
	if (boardModel->isShuffleNeed())
	{
		boardModel->addAvailableMove();
	}
}


void BoardController::showShuffleAction()
{
	shuffleAnimation->setVisible(true);
	shuffleAnimation->playAnimation();
	scheduleOnce(schedule_selector(BoardController::shuffle), 0.5);
}


int BoardController::canSwapTiles(Cell* selectedCell, Cell* targetCell, bool addToCrush)
{
	auto selectedTileType = selectedCell->getSourceTile()->getMovingTileType();
	auto targetTileType = targetCell->getSourceTile()->getMovingTileType();

	if(Utils::IsBonusTile(selectedTileType) && Utils::IsBonusTile(targetTileType))
	{
		matchId = 0;

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
	if(selectedTileType == +MovingTileTypes::RainbowObject)
	{
		matchId = 0;

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
	if (targetTileType == +MovingTileTypes::RainbowObject)
	{
		matchId = 0;

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
			matchId = 0;

			canSwap = true;
			matchSelected->matchId = getMatchId();
			selectedCell->getSourceTile()->matchId = matchSelected->matchId;
			matchSelected->isWaiting = true;
			pendingCrushCells->addObject(matchSelected);
		}
		if (matchTarget != nullptr)
		{
			matchId = 0;

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

void BoardController::addScore(ScoreType type, ScoreUnit val, char matchNumber, Vec2 pos) const
{
	Score score(type, val, matchNumber);
	score.setPosition(pos);
	scoreController->addScore(score);
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

	if (squareMatchedDir >= 0 && squareMatchedDir < 4)
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

void BoardController::initHintAction()
{
	hintTime = HINT_TIME;
	boardModel->clearAvailableMove();
}

void BoardController::countDownMoveNumber()
{
	moveCount++;
	updateMoveCountText();
}

void BoardController::updateMoveCountText()
{
	moveCountNode->setString(StringUtils::format("%d", currentLevel->getMoveCount() - moveCount));
}

void BoardController::doSomethingPerMove()
{
	countDownMoveNumber();
	boardModel->setIncreaseLavaCakeFlag(true);
	moveConveyorsFlag = true;
	moveSpinnerFlag = true;
	moveSwappersFlag = true;
	spreedHoneyModifierFlag = true;
}

void BoardController::crushPendingCells()
{
	if(pendingCrushCells == nullptr || pendingCrushCells->count() == 0 || gameState != GameState::Idle)
	{
		return;
	}
	auto match = static_cast<Match*>(pendingCrushCells->getObjectAtIndex(0));
	if(!match->isWaiting)
	{
		crushMatch(match);
		pendingCrushCells->removeObjectAtIndex(0);
	}

	//Ref* itr = nullptr;
	//CCARRAY_FOREACH(pendingCrushCells, itr)
	//{
	//	const auto match = static_cast<Match*>(itr);
	//	if (!match->isWaiting)
	//	{
	//		crushMatch(match);
	//		pendingCrushCells->fastRemoveObject(match);
	//	}
	//}

}

void BoardController::playCreateBonusSoundEffect(MovingTileTypes bonusType)
{
	auto soundEffectName = SoundEffects::sound_create_row_column;

	switch (bonusType)
	{
	case MovingTileTypes::BombBreakerObject:
		soundEffectName = SoundEffects::sound_create_bomb;
		break;
	case MovingTileTypes::SeekerObject:
		soundEffectName = SoundEffects::sound_create_honey;
		break;
	case MovingTileTypes::RainbowObject:
		soundEffectName = SoundEffects::sound_create_rainbow;
		break;
	case MovingTileTypes::RowBreakerObject:
	case MovingTileTypes::ColumnBreakerObject:
		soundEffectName = SoundEffects::sound_create_row_column;
		break;
	case MovingTileTypes::XBreakerObject:
		soundEffectName = SoundEffects::sound_create_x;
		break;
	}
	soundController->playEffectSound(soundEffectName);
}


void BoardController::crushNormalMatch(Match* match)
{
	auto bonusType = match->getAvailableBonusType();
	if (match->refCell->getSourceTile() == nullptr)
	{
		if (bonusType != +MovingTileTypes::LayeredMatchObject && !match->checkBonusCreated(bonusType))
		{
			auto bonusTile = poolController->getCookieTile(bonusType._to_string());
			bonusTile->initWithType(bonusType._to_string(), match->color);
			match->refCell->setSourceTile(bonusTile);
			match->refCell->reserveCount = 1;
			playCreateBonusSoundEffect(bonusType);
			addScore(ScoreType::additive, ScoreUnit::rainbow, match->matchId, match->refCell->getBoardPos());
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
	soundController->playEffectSound("sound_gem_match_tier", match->matchId);

	if (bonusType != +MovingTileTypes::LayeredMatchObject)
	{
		auto bonusTile = poolController->getCookieTile(bonusType._to_string());
		bonusTile->initWithType(bonusType._to_string(), color);
		match->refCell->setSourceTile(bonusTile);
		bonusTile->setPosition(match->refCell->getBoardPos());
		match->refCell->reserveCount = 1;
		playCreateBonusSoundEffect(bonusType);
		addScore(ScoreType::additive, ScoreUnit::rainbow, match->matchId, match->refCell->getBoardPos());
	}
	else
	{
		addScore(ScoreType::additive, ScoreUnit::match, match->matchId, match->refCell->getBoardPos());
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
			combineXAndLine(match->refCell, match->bonusMatchCell);
			break;
		case MovingTileTypes::BombBreakerObject:
			combineXAndLine(match->refCell, match->bonusMatchCell);
			break;
		case MovingTileTypes::RainbowObject:
			combineRainbowAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::SeekerObject:
			combineSeekerAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			combineXAndLine(match->refCell, match->bonusMatchCell);
			break;
		default:
			break;
		}
		break;
	case MovingTileTypes::BombBreakerObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::XBreakerObject:
			combineXAndLine(match->refCell, match->bonusMatchCell);
			break;
		case MovingTileTypes::BombBreakerObject:
			crushTwoBomb(match->refCell);
			break;
		case MovingTileTypes::RainbowObject:
			combineRainbowAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::SeekerObject:
			combineSeekerAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			combineBombAndLine(match->refCell, match->bonusMatchCell);
			break;
		default:
			break;
		}
		break;
	case MovingTileTypes::RainbowObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::RainbowObject:
			combine2Rainbow(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::XBreakerObject:
		case MovingTileTypes::BombBreakerObject:
		case MovingTileTypes::SeekerObject:
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			combineRainbowAndBonus(match->refCell, match->bonusMatchCell);
			break;
		default:
			break;
		}
		break;
	case MovingTileTypes::SeekerObject:
		switch (targetCellBonusType)
		{
		case MovingTileTypes::XBreakerObject:
			combineSeekerAndBonus(match->refCell, match->bonusMatchCell);
			break;
		case MovingTileTypes::BombBreakerObject:
			combineSeekerAndBonus(match->refCell, match->bonusMatchCell);
			break;
		case MovingTileTypes::RainbowObject:
			combineRainbowAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::SeekerObject:
			combineSeekerAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			combineSeekerAndBonus(match->refCell, match->bonusMatchCell);
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
			combineXAndLine(match->refCell, match->bonusMatchCell);
			break;
		case MovingTileTypes::BombBreakerObject:
			combineBombAndLine(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::RainbowObject:
			combineRainbowAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::SeekerObject:
			combineSeekerAndBonus(match->bonusMatchCell, match->refCell);
			break;
		case MovingTileTypes::ColumnBreakerObject:
		case MovingTileTypes::RowBreakerObject:
			combine2LineBreakers(match->refCell, match->bonusMatchCell);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void BoardController::combine2LineBreakers(Cell* refCell, Cell* targetCell)
{
	targetCell->crushCell();
	crushColumnBreaker(refCell);
	crushRowBreaker(refCell);
}

void BoardController::combineXAndLine(Cell* refCell, Cell* targetCell)
{
	targetCell->crushCell();
	crushColumnBreaker(refCell);
	crushRowBreaker(refCell);
	crushXBreaker(refCell);
}

void BoardController::crushRainbowMatch(Match* match)
{
	match->refCell->crushCell();
	auto sameColorCells = boardModel->getSameColorCells(match->color);
	for(auto cell : *sameColorCells)
	{
		showRainbowLineEffect(match->refCell, cell);
		//crushCell(cell);
	}
	CC_SAFE_DELETE(sameColorCells);
}

void BoardController::showRainbowLineEffect(Cell* refCell, Cell*targetCell)
{
	auto line = poolController->getLightLine();
	auto circle = poolController->getLightCircle();
	circle->setPosition(targetCell->getBoardPos());
	showObjectsLayer->addChild(line);
	showObjectsLayer->addChild(circle);
	line->setPosition(refCell->getBoardPos());
	CKAction ckAction;
	fallingTileCount++;
	ckAction.node = reinterpret_cast<Node*>(circle);
	ckAction.action = actionController->creatLightCircleShowAction([=] {
		PoolController::getInstance()->recycleLightCircle(circle);
		this->crushCell(targetCell);
		this->fallingTileCount--;
	}, ckAction.node);
	actionController->pushAction(ckAction, true);

	auto rotation = Math::getRotation(refCell->getBoardPos(), targetCell->getBoardPos()) - 90;
	auto length = refCell->getBoardPos().distance(targetCell->getBoardPos());
	line->setAnchorPoint(Vec2(0, 0.5f));
	line->setContentSize(Size(length, 32));
	line->setRotation(rotation);
	CKAction lineAction;
	lineAction.node = reinterpret_cast<Node*>(line);
	lineAction.action = actionController->creatLightCircleShowAction([=] {
		PoolController::getInstance()->recycleLightLine(line);
	}, lineAction.node);
	actionController->pushAction(lineAction, true);
}

void BoardController::combineSeekerAndBonus(Cell* seekerCell, Cell* bonusCell)
{
	if(bonusCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::SeekerObject)
	{
		crushSeeker(seekerCell);
		crushSeeker(seekerCell);
		crushSeeker(bonusCell);
	}
	else
	{
		crushSeekerAndBonus(seekerCell, bonusCell);
	}
	seekerCell->crushCell(false);
	bonusCell->crushCell(false);
}

void BoardController::combineRainbowAndBonus(Cell* rainbowCell, Cell* bonusCell)
{
	auto bonusType = bonusCell->getMovingTile()->getMovingTileType();
	auto tileColor = bonusCell->getMovingTile()->getTileColor();
	auto sameColorCells = boardModel->getSameColorCells(tileColor);
	for (auto cell : *sameColorCells)
	{
		cell->crushCell(false);
		addMovingTile(cell, bonusType, tileColor);
		auto match = Match::create();
		match->matchType = SingleCellMatch;
		match->refCell = cell;
		match->retain();
		pendingCrushCells->addObject(match);
	}
}

void BoardController::combine2Rainbow(Cell* first, Cell* second)
{
	showBombCrushEffect(first);
	crushAllCells();
}

void BoardController::combineBombAndLine(Cell* refCell, Cell* targetCell)
{
	targetCell->crushCell();
	crushColumnBreaker(refCell);
	crushColumnBreaker(refCell->rightCell); 
	crushColumnBreaker(refCell->leftCell);
	crushRowBreaker(refCell);
	crushRowBreaker(refCell->upCell);
	crushRowBreaker(refCell->downCell);
	showBombAndLineCrushEffect(refCell);
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
		case SingleCellMatch:
			crushCell(match->refCell);
			break;
		default:
			break;
		}
	}
}

void BoardController::crushAllCells()
{
	for(char i = 0; i < boardModel->getHeight(); i++)
	{
		for(char j = 0; j < boardModel->getWidth(); j++)
		{
			auto cell = getMatchCell(j, i);
			if(cell != nullptr && !cell->isOutCell)
			{
				cell->crushCell(true);
			}
		}
	}
}

void BoardController::spawnNewTile(Cell* cell)
{
	auto spawnSys = boardModel->getSpawnOnCollectSystem();
	auto spawner = cell->getSpawner();
	auto spawnerName = spawner->getTileName();
	if (spawnSys != nullptr && (spawnerName == "normal" || spawnerName == ""))
	{
		auto tileCount = boardModel->getLiquidFillersCount(spawnSys->ObjectType == (+MovingTileTypes::LiquidFillerMatchObject)._to_string());
		if (tileCount < spawnSys->EnsureOne)
		{
			if ((spawnSys->ObjectType == (+MovingTileTypes::LiquidFillerMatchObject)._to_string() && boardModel->getLiquidSystem()->LevelMax <= boardModel->getCurrentLiquidLevel())
				|| (spawnSys->ObjectType == (+MovingTileTypes::LiquidDrainerMatchObject)._to_string() && boardModel->getLiquidSystem()->LevelMin >= boardModel->getCurrentLiquidLevel()))
			{
				cell->spawnMatchTile();
			}
			else
			{
				cell->spawnSpecialTile(MovingTileTypes::_from_string(spawnSys->ObjectType.c_str()));
			}
			return;
		}
	}
	cell->spawnMatchTile();
}


Cell* BoardController::fillCell(Cell* cell)
{
	if(cell->containsDisplayCase())
	{
		cell->fillDisplayCase();
		return nullptr;
	}
	auto fallPath = findFallPath(cell);
	if (fallPath->startCell != nullptr)
	{
		Cell* movedCell = nullptr;
		if (fallPath->startCell->isEmpty || (fallPath->startCell->isFixed && fallPath->startCell->canPass))
		{
			if (fallPath->startCell->containsSpawner() && fallPath->startCell->fallDirection == fallPath->startCell->getSpawner()->getDirection())
			{
				spawnNewTile(fallPath->startCell);
			}
		}
		if(fallPath->containsPortal)
		{
			movedCell = fallPath->startCell;
		}

		//////////////////////////////////////
		// fallPath->log();
		fallPath->showFallAction();
		soundController->playEffectSound(SoundEffects::sound_gem_fall);
		auto newTile = fallPath->startCell->getSpawnedTile();
		if(newTile == nullptr)
		{
			return nullptr;
		}
		cell->setSourceTile(newTile);
		newTile->setCellPos();

		if (!Utils::IsSameGrid(fallPath->startCell->gridPos, cell->gridPos))
		{
			if (fallPath->startCell->containsSpawner() && fallPath->startCell->isFixed)
			{
				fallPath->startCell->resetSpawnedTile();
			}
			else
			{
				fallPath->startCell->clear();
			}
		}

		//if (!fallPath->startCell->isEmpty || fallPath->startCell->containsSpawner())
		//{
		//	//if (fallPath->startCell->getMovingTile() != nullptr && !fallPath->startCell->getMovingTile()->isMoving)
		//	//{
		//	//	fallPath->showFallAction();
		//	//}
		//	fallPath->showFallAction();

		//	cell->setSourceTile(fallPath->startCell->getSpawnedTile());
		//	if (!Utils::IsSameGrid(fallPath->startCell->gridPos, cell->gridPos))
		//	{
		//		if(fallPath->startCell->containsSpawner() && fallPath->startCell->isFixed)
		//		{
		//			fallPath->startCell->resetSpawnedTile();
		//		}
		//		else
		//		{
		//			fallPath->startCell->clear();
		//		}
		//	}
		//	cell->getSourceTile()->setCellPos();
		//}
		return movedCell;
	}
	else
	{
		
	}
	return nullptr;
}

bool BoardController::checkPastHole(Cell* cell, char refCol, char refRow, bool inWater)
{
	if(cell->getMovingTile() != nullptr && cell->getMovingTile()->isMoving)
	{
		return false;
	}
	if(inWater)
	{
		if (cell->gridPos.Row > refRow)
			return true;
		if (cell->gridPos.Row < refRow)
			return false;
	}
	else
	{
		if (cell->gridPos.Row < refRow)
			return true;
		if (cell->gridPos.Row > refRow)
			return false;
	}
	return cell->gridPos.Col < refCol;
}

bool BoardController::fallTiles()
{
	auto portalPassed = false;

	/// out water search
	for (char i = 0; i < boardModel->getHeight(); i++)
	{
		for (char j = 0; j < boardModel->getWidth(); j++)
		{
			auto cell = boardModel->getCell(j, i);
			if(cell->isEmpty && cell->isFillable && !cell->isOutCell && !cell->inWater)
			{
				auto movedCell = fillCell(cell);
				if(movedCell != nullptr && movedCell->isEmpty && checkPastHole(movedCell, j, i))
				{
					portalPassed = true;
				}
			}
		}
	}

	for (char i = boardModel->getHeight() - 1; i >= 0; i--)
	{
		for (char j = 0; j < boardModel->getWidth(); j++)
		{
			auto cell = boardModel->getCell(j, i);
			if (cell->isEmpty && cell->isFillable && !cell->isOutCell && cell->inWater)
			{
				auto movedCell = fillCell(cell);
				if (movedCell != nullptr && movedCell->isEmpty && checkPastHole(movedCell, j, i))
				{
					portalPassed = true;
				}
			}
		}
	}

	/// in water search

	return portalPassed;
}

void BoardController::fallTilesLoop()
{
	if (fallingTileCount > 0 || gameState != Idle)
	{
		return;
	}

	if(fallTiles())
	{
		//fallTiles();
	}

	//auto loopCount = 3;
	//while(loopCount-- > 0)
	//{
	//	if(!fallTiles())
	//	{
	//		break;
	//	}
	//}
	//cocos2d::log("loop count: %d", loopCount);
}

FallPath* BoardController::findFallPath(Cell* cell)
{
	auto fallPath = new FallPath;
	fallPath->inWater = cell->inWater;
	fallPath->endCell = cell;
	if (cell->containsPortalOut()) 
	{
		fallPath->containsPortal = true;
		auto fallCell = cell->findPortalInCell(boardModel->portalInList);
		if(fallCell->canFall())
		{
			fallPath->startCell = fallCell;
			return fallPath;
		}
		if (fallPath->startCell == nullptr)
		{
			fallPath->containsPortal = false;
		}
	}
	if(cell->containsSpawner() && !cell->containsPortalOut())
	{
		fallPath->startCell = cell;
		return fallPath;
	}

	auto targetCell = cell;
	auto fallCell = boardModel->getDirectFallCell(cell);

	if (fallCell->canFall())
	{
		fallPath->startCell = fallCell;
		return fallPath;
	}

	if (fallCell->containsPortalOut())
	{
		fallPath->containsPortal = true;
		fallPath->pushCell(fallCell);
		fallCell = fallCell->findPortalInCell(boardModel->portalInList);
		if (fallCell->canFall()) 
		{
			fallPath->startCell = fallCell;
		}
		else if (fallCell->canFill()) 
		{
			auto portalPath = findFallPath(fallCell);
			if (portalPath != nullptr && portalPath->startCell != nullptr && (portalPath->startCell->canFall() || portalPath->startCell->containsSpawner()))
			{
				fallPath->addPath(portalPath);
			}
			CC_SAFE_DELETE(portalPath);
		}
		if(fallPath->startCell == nullptr)
		{
			fallCell = fallPath->fallPath.back();
			fallPath->containsPortal = false;
		}
		else
		{
			return fallPath;
		}
	}
	else if (fallCell->containsSpawner() && !cell->containsPortalOut())
	{
		fallPath->startCell = fallCell;
		return fallPath;
	}

	//Cell* inclinedFallCell = boardModel->getInclinedFallCell(fallCell);
	do
	{
		Cell* inclinedFallCell = boardModel->getInclinedFallCell(fallCell);
		if (inclinedFallCell != nullptr) 
		{
			if (inclinedFallCell->canFall() || (inclinedFallCell->containsSpawner() && !inclinedFallCell->containsPortalOut()) )
			{
				fallPath->pushCell(fallCell);
				fallPath->startCell = inclinedFallCell;
				return fallPath;
			}
			if (inclinedFallCell->canFill()) 
			{
				auto newFallPath = findFallPath(inclinedFallCell);
				if (newFallPath != nullptr && newFallPath->startCell != nullptr && (newFallPath->startCell->canFall() || newFallPath->startCell->containsSpawner()) ) 
				{
					fallPath->addPath(newFallPath);
					CC_SAFE_DELETE(newFallPath);
					return fallPath;
				}
				else 
				{
					auto oppIncFallCell = inclinedFallCell->gridPos.Col < fallCell->gridPos.Col ? getMatchCell(fallCell->gridPos.Col + 1, inclinedFallCell->gridPos.Row) :
						getMatchCell(fallCell->gridPos.Col - 1, inclinedFallCell->gridPos.Row);
					if (oppIncFallCell != nullptr && oppIncFallCell->canFill()) 
					{
						auto oppFallPath = findFallPath(oppIncFallCell);
						if (oppFallPath != nullptr && oppFallPath->startCell != nullptr && (oppFallPath->startCell->canFall() || oppFallPath->startCell->containsSpawner()))
						{
							fallPath->addPath(oppFallPath);
							CC_SAFE_DELETE(oppFallPath);
							return fallPath;
						}
						CC_SAFE_DELETE(oppFallPath);
					}
				}
				CC_SAFE_DELETE(newFallPath);
			}
		}
		fallCell = fallCell->getFallDownCell();
	} while (fallCell != nullptr 
		&& fallCell->inWater == cell->inWater
		&& fallCell->gridPos.Row * (fallCell->inWater ? -1 : 1 )>= cell->gridPos.Row * (fallCell->inWater ? -1 : 1) // In water the direction of fall is negative.
		&& (fallCell->canFill() || fallCell->canPass));

	return fallPath;
}

void BoardController::crushNearbyCells(Cell* cell, TileColors color)
{
	if (cell->upCell != nullptr && cell->upCell->isReceiveNearbyAffect())
	{
		cell->upCell->setNearbyCrushColor(color);
		crushCell(cell->upCell);
	}
	if (cell->rightCell != nullptr && cell->rightCell->isReceiveNearbyAffect())
	{
		cell->rightCell->setNearbyCrushColor(color);
		crushCell(cell->rightCell);
	}
	if (cell->downCell != nullptr && cell->downCell->isReceiveNearbyAffect())
	{
		cell->downCell->setNearbyCrushColor(color);
		crushCell(cell->downCell);
	}
	if (cell->leftCell != nullptr && cell->leftCell->isReceiveNearbyAffect())
	{
		cell->leftCell->setNearbyCrushColor(color);
		crushCell(cell->leftCell);
	}
}

void BoardController::crushUnderCells(Cell* cell)
{
	
}


void BoardController::crushCell(Cell* cell, bool forceClear)
{
	if (cell == nullptr || cell->isEmpty || cell->getMovingTile() == nullptr)
	{
		return;
	}

	auto tile = cell->getMovingTile();
	auto tileType = tile->getMovingTileType();
	auto strType = tile->getType();
	auto canMatch = tile->canMatch;
	auto tileColor = tile->getTileColor();
	auto containsHoneyModifier = tile->containsHoneyModifier();

	if(!cell->crushCell())
	{
		if (containsHoneyModifier)
		{
			spreedHoneyModifierFlag = false;
		}
		//if (forceClear) cell->clear();
		return;
	}

	//if (forceClear) cell->clear();
	if(canMatch)
	{
		crushUnderCells(cell);
		crushNearbyCells(cell, tileColor);
	}

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
	case MovingTileTypes::ChocolateCheesecakeObject:
		crushDirectionalBreaker(cell, tile->getDirection());
		break;
	case MovingTileTypes::LiquidDrainerMatchObject:
		fillLiquid(true);
		break;
	case MovingTileTypes::LiquidFillerMatchObject:
		fillLiquid();
		break;
	case MovingTileTypes::PathMoverMatchObject:
		pathMoverCollected(cell);
		break;
	default:
		break;
	}

	if (strType == LAVACAKEOBJECT)
	{
		auto lavaCakeTagets = boardModel->getLavaCakeTargets();
		spawnLavaCake(cell, lavaCakeTagets);
	}
	else if(strType.find("ColorPie") != std::string::npos)
	{
		crushBombBreaker(cell);
	}
}

void BoardController::spawnLavaCake(Cell* cell, CellsList* targets)
{
	if (targets == nullptr || targets->size() == 0)
	{
		return;
	}

	for(auto targetCell : *targets)
	{
		auto spawnColor = spawnController->getSpawnColor();
		auto lavaCakeTile = cell->getSourceTile();
		auto spawnType = lavaCakeTile->getSpawnType();
		auto spawnData = lavaCakeTile->getSpawnData();
		auto spawnTile = poolController->getCookieTile(spawnType);
		spawnTile->setTileColor(spawnColor);
		spawnTile->setLayers(spawnData.Layers);
		spawnTile->setDirection(spawnData.direction);
		spawnTile->initWithGrid(targetCell->gridPos.Col, targetCell->gridPos.Row);
		spawnTile->setPosition(cell->getBoardPos());
		spawnTile->initWithType(spawnType, spawnColor);
		effectNode->addChild(spawnTile);

		CKAction ckAction;
		ckAction.node = spawnTile;
	
		ckAction.action = actionController->createJumpAction(spawnTile, targetCell->getBoardPos(), CellSize, [=] {
			auto originTile = targetCell->getSourceTile();
			if (originTile != nullptr && originTile->getType() == CHOCOLATEOBJECT && spawnTile->getType() == CHOCOLATEOBJECT)
			{
				auto lavaTile = (LavaCakeObject*)originTile;
				lavaTile->addLayers(spawnTile->getLayers());
				poolController->recycleCookieTile(spawnTile);
			}
			else
			{
				crushCell(targetCell, true);
				if (targetCell->getSourceTile() != nullptr && targetCell->getSourceTile()->getParent() != nullptr)
				{
					poolController->recycleCookieTile(targetCell->getSourceTile());
				}
				targetCell->setSourceTile(spawnTile);
				spawnTile->removeFromParent();
				layeredMatchLayer->addChild(spawnTile);
				spawnTile->setPosition(targetCell->getBoardPos());
			}
			fallingTileCount--;
		});
		fallingTileCount++;
		actionController->pushAction(ckAction, false);
	}
}

void BoardController::moveSpinners()
{
	if (fallingTileCount > 0 || gameState != Idle || pendingCrushCells->count() > 0 || !moveSpinnerFlag)
	{
		return;
	}

	moveSpinnerFlag = false;
	boardModel->runObjectSpinner();
}

void BoardController::moveSwappers()
{
	if (fallingTileCount > 0 || gameState != Idle || pendingCrushCells->count() > 0 || !moveSwappersFlag)
	{
		return;
	}

	moveSwappersFlag = false;
	boardModel->runSwappers();
}

void BoardController::spreedHoneyModifier()
{
	if (fallingTileCount > 0 || gameState != Idle || pendingCrushCells->count() > 0 || !boardModel->containsHoneyModifier() || !spreedHoneyModifierFlag)
	{
		return;
	}
	spreedHoneyModifierFlag = false;
	auto honeyTargetCell = boardModel->findHoneyTarget();
	if (honeyTargetCell != nullptr && honeyTargetCell->getMovingTile() != nullptr)
	{
		honeyTargetCell->getMovingTile()->setHoneyModifier();
	}
}


void BoardController::fillLiquid(bool inverse)
{
	auto curretLiquidLevel = boardModel->getCurrentLiquidLevel();
	if (curretLiquidLevel >= boardModel->getLiquidSystem()->LevelMax && !inverse)
	{
		return;
	}
	if (curretLiquidLevel <= 0 && inverse)
	{
		return;
	}
	if(!inverse) // fill
	{
		boardModel->setCurrentLiquidLevel(curretLiquidLevel + 1);
	}
	else // drain
	{
		boardModel->setCurrentLiquidLevel(curretLiquidLevel - 1);
	}
	
	curretLiquidLevel = boardModel->getCurrentLiquidLevel();
	auto liquidSystem = boardModel->getLiquidSystem();
	if(curretLiquidLevel >= liquidSystem->LevelMax && liquidSystem->FillerToggle)
	{
		toggleLiquidFiller((+MovingTileTypes::LiquidFillerMatchObject)._to_string());
	}

	if (curretLiquidLevel <= liquidSystem->LevelMin && liquidSystem->DrainerToggle)
	{
		toggleLiquidFiller((+MovingTileTypes::LiquidDrainerMatchObject)._to_string());
	}
	//liquidNode->stopAllActions();
	auto action = MoveTo::create(0.5, Vec2(0, boardModel->getCurrentLiquidLevel() * CellSize));
	action->setTag(6666);
	liquidNode->stopActionByTag(6666);
	liquidNode->runAction(action);
}


void BoardController::crushBombBreaker(Cell* cell)
{
	for(char i = cell->gridPos.Row - 2; i <= cell->gridPos.Row + 2; i++)
	{
		for(char j = cell->gridPos.Col - 2; j <= cell->gridPos.Col + 2; j++)
		{
			auto bombCell = getMatchCell(j, i);
			if(bombCell == nullptr || bombCell == cell) continue;
			if ((std::abs(i - cell->gridPos.Row) == 2)) 
			{ 
				if(std::abs(j - cell->gridPos.Col) != 0)
					continue; 
			}
			if ((std::abs(j - cell->gridPos.Col) == 2))
			{
				if (std::abs(i - cell->gridPos.Row) != 0)
					continue;
			}
			crushCell(bombCell);
		}
	}
	showBombCrushEffect(cell);
	soundController->playEffectSound(SoundEffects::sound_explode_bomb);
}

void BoardController::crushTwoBomb(Cell* cell)
{
	for (char i = cell->gridPos.Row - 3; i <= cell->gridPos.Row + 3; i++)
	{
		for (char j = cell->gridPos.Col - 3; j <= cell->gridPos.Col + 3; j++)
		{
			auto bombCell = getMatchCell(j, i);
			if (bombCell == nullptr) continue;
			if ((std::abs(i - cell->gridPos.Row) == 3))
			{
				if (std::abs(j - cell->gridPos.Col) != 0)
					continue;
			}
			if ((std::abs(j - cell->gridPos.Col) == 3))
			{
				if (std::abs(i - cell->gridPos.Row) != 0)
					continue;
			}
			crushCell(bombCell);
		}
	}
	showBombCrushEffect(cell);
	soundController->playEffectSound(SoundEffects::sound_explode_double_bomb);
}

void BoardController::crushDirectionalBreaker(Cell* cell, Direction direction)
{
	auto rot = 0.f;
	switch(direction)
	{
		case Direction::N:
		{
			rot = -90;
			auto upCell = cell->upCell;
			while (upCell != nullptr)
			{
				if (!upCell->isEmpty && upCell->getMovingTile() != nullptr)
				{
					if (upCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::ColumnBreakerObject)
					{
						upCell->getMovingTile()->setMovingTileType(MovingTileTypes::RowBreakerObject);
					}
					crushCell(upCell);
				}
				upCell = upCell->upCell;
			}
			break;
		}
		case Direction::NE:
		{
			rot = -45;
			Cell* upRightCell = cell;
			while (upRightCell != nullptr && upRightCell->upCell != nullptr && upRightCell->upCell->rightCell != nullptr)
			{
				crushCell(upRightCell->upCell->rightCell);
				upRightCell = upRightCell->upCell->rightCell;
			}
			break;
		}
		case Direction::E:
		{
			rot = 0;
			auto rightCell = cell->rightCell;
			while (rightCell != nullptr)
			{
				if (!rightCell->isEmpty && rightCell->getMovingTile() != nullptr)
				{
					if (rightCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::RowBreakerObject)
					{
						rightCell->getMovingTile()->setMovingTileType(MovingTileTypes::ColumnBreakerObject);
					}
					crushCell(rightCell);
				}
				rightCell = rightCell->rightCell;
			}
			break;
		}
		case Direction::SE:
		{
			rot = 45;
			Cell* downRightCell = cell;
			while (downRightCell != nullptr && downRightCell->downCell != nullptr && downRightCell->downCell->rightCell != nullptr)
			{
				crushCell(downRightCell->downCell->rightCell);
				downRightCell = downRightCell->downCell->rightCell;
			}
			break;
		}
		case Direction::S:
		{
			rot = 90;
			auto downCell = cell->downCell;
			while (downCell != nullptr)
			{
				if (!downCell->isEmpty && downCell->getMovingTile() != nullptr)
				{
					if (downCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::ColumnBreakerObject)
					{
						downCell->getMovingTile()->setMovingTileType(MovingTileTypes::RowBreakerObject);
					}
					crushCell(downCell);
				}
				downCell = downCell->downCell;
			}
			break;
		}
		case Direction::SW:
		{
			rot = 135;
			Cell* downLeftCell = cell;
			while (downLeftCell != nullptr && downLeftCell->downCell != nullptr && downLeftCell->downCell->leftCell != nullptr)
			{
				crushCell(downLeftCell->downCell->leftCell);
				downLeftCell = downLeftCell->downCell->leftCell;
			}
			break;
		}
		case Direction::W:
		{
			rot = 180;
			auto leftCell = cell->leftCell;
			while (leftCell != nullptr)
			{
				if (!leftCell->isEmpty && leftCell->getMovingTile() != nullptr)
				{
					if (leftCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::RowBreakerObject)
					{
						leftCell->getMovingTile()->setMovingTileType(MovingTileTypes::ColumnBreakerObject);
					}
					crushCell(leftCell);
				}
				leftCell = leftCell->leftCell;
			}
			break;
		}
		case Direction::NW:
		{
			rot = 225;
			Cell* upLeftCell = cell;
			while (upLeftCell != nullptr && upLeftCell->upCell != nullptr && upLeftCell->upCell->leftCell != nullptr)
			{
				crushCell(upLeftCell->upCell->leftCell);
				upLeftCell = upLeftCell->upCell->leftCell;
			}
			break;
		}
	}
	showLineCrushEffect(cell, rot, 1);
	soundController->playEffectSound(SoundEffects::sound_explode_row_column);
}

void BoardController::crushRowBreaker(Cell* cell, bool showEffect)
{
	if(cell == nullptr)
	{
		return;
	}
	auto leftCell = cell->leftCell;
	cell->crushCell();
	while (leftCell != nullptr)
	{
		if (!leftCell->isEmpty && leftCell->getMovingTile() != nullptr)
		{
			if (leftCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::RowBreakerObject)
			{
				leftCell->getMovingTile()->setMovingTileType(MovingTileTypes::ColumnBreakerObject);
			}
			crushCell(leftCell);

		}		
		leftCell = leftCell->leftCell;
	}

	auto rightCell = cell->rightCell;
	while (rightCell != nullptr)
	{
		if (!rightCell->isEmpty && rightCell->getMovingTile() != nullptr)
		{
			if (rightCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::RowBreakerObject)
			{
				rightCell->getMovingTile()->setMovingTileType(MovingTileTypes::ColumnBreakerObject);
			}
			crushCell(rightCell);

		}		
		rightCell = rightCell->rightCell;
	}

	if(showEffect) showLineCrushEffect(cell, 0);
	soundController->playEffectSound(SoundEffects::sound_explode_row_column);
}

void BoardController::crushColumnBreaker(Cell* cell, bool showEffect)
{
	auto upCell = cell->upCell;
	cell->crushCell();
	while (upCell != nullptr)
	{
		if(!upCell->isEmpty && upCell->getMovingTile() != nullptr)
		{
			if(upCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::ColumnBreakerObject)
			{
				upCell->getMovingTile()->setMovingTileType(MovingTileTypes::RowBreakerObject);
			}
			crushCell(upCell);
		}
		upCell = upCell->upCell;
	}

	auto downCell = cell->downCell;
	while (downCell != nullptr)
	{
		if (!downCell->isEmpty && downCell->getMovingTile() != nullptr)
		{
			if (downCell->getMovingTile()->getMovingTileType() == +MovingTileTypes::ColumnBreakerObject)
			{
				downCell->getMovingTile()->setMovingTileType(MovingTileTypes::RowBreakerObject);
			}
			crushCell(downCell);

		}	
		downCell = downCell->downCell;
	}
	if (showEffect) showLineCrushEffect(cell, 90);
	soundController->playEffectSound(SoundEffects::sound_explode_row_column);
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
	showLineCrushEffect(cell, 45);
	showLineCrushEffect(cell, 135);
	soundController->playEffectSound(SoundEffects::sound_explode_x);
}

void BoardController::crushSeeker(Cell* cell, MovingTileTypes bonusType)
{
	auto randomPos = boardModel->getRandomBoardPosition();
	auto seekerShow = poolController->getSeekerShow(cell->tileColor);
	seekerShow->strData = bonusType;
	showObjectsLayer->addChild(seekerShow);
	seekerShow->setPosition(cell->getBoardPos());
	CKAction ckAction;
	ckAction.node = static_cast<Node*>(seekerShow);
	ckAction.action = actionController->createSeekerPendingAction(ckAction.node, randomPos);
	actionController->pushAction(ckAction, false);
	pendingSeekers->addObject(seekerShow);
	cell->crushCell();
	soundController->playEffectSound(SoundEffects::sound_explode_honey);
}

void BoardController::crushSeekerAndBonus(Cell* seekerCell, Cell* bonusCell)
{
	auto randomPos = boardModel->getRandomBoardPosition();
	auto seekerShow = poolController->getSeekerShow(seekerCell->tileColor);
	auto bonusType = bonusCell->getMovingTile()->getType();
	seekerShow->strData = bonusType;
	cocos2d::log("crush seeker and bonus: %s", bonusType.c_str());
	showObjectsLayer->addChild(seekerShow);
	seekerShow->setPosition(seekerCell->getBoardPos());
	CKAction ckAction;
	ckAction.node = static_cast<Node*>(seekerShow);
	ckAction.action = actionController->createSeekerPendingAction(ckAction.node, randomPos);
	actionController->pushAction(ckAction, false);
	pendingSeekers->addObject(seekerShow);
	seekerCell->crushCell();

	auto bonusTile = bonusCell->getMovingTile();
	
	auto showObj = poolController->getTileShowObject();
	showObj->setSpriteFrame(bonusTile->getSpriteFrame());
	showObj->setPosition(bonusCell->getBoardPos());
	showObj->setAnchorPoint(Vec2(0.5, 0.5));
	showObjectsLayer->addChild(showObj);
	CKAction ckActionB;
	ckActionB.node = static_cast<Node*>(showObj);
	ckActionB.action = actionController->createSeekerAndBonusPendingAction(ckAction.node, randomPos, [=]{
		showObj->removeFromParent();
		showObj->setScale(0.6);
		seekerShow->addChild(showObj, 200, "bonusChild");
		showObj->setPosition(Vec2(0, 0));
	});
	actionController->pushAction(ckActionB, false);
}

void BoardController::processPendingSeekers()
{
	if(fallingTileCount > 0 || gameState != GameState::Idle)
	{
		return;
	}
	CellsList targetsList;
	Ref* itr = nullptr;
	CCARRAY_FOREACH(pendingSeekers, itr)
	{
		auto seekerShow = static_cast<AnimationShowObject*>(itr);
		auto targetCell = findSeekerTarget(&targetsList);
		while(Utils::containsCell(&targetsList, targetCell))
		{
			targetCell = findSeekerTarget(&targetsList);
		}
		//if(targetCell == nullptr) continue;
		//if(targetCell->getSourceTile() == nullptr) continue;
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
	//auto crushingCell = targetCell;
	//auto recycleSeeker = seekerShow;
	ckAction.action = actionController->createSeekerLandingAction(ckAction.node, targetPos, [=]()
	{
		auto bonusString = seekerShow->strData;
		this->crushCell(targetCell);
		this->crushBonusManually(targetCell, bonusString);
		poolController->recycleSeekerShow(seekerShow);
		soundController->playEffectSound(SoundEffects::sound_gem_landing);
	});
	actionController->pushAction(ckAction, false);
}

void BoardController::crushBonusManually(Cell* cell, std::string bonusString)
{
	cocos2d::log("seeker bonus: %s", bonusString.c_str());
	if (!MovingTileTypes::_is_valid(bonusString.c_str()))
	{
		return;
	}
	auto bonusType = MovingTileTypes::_from_string(bonusString.c_str());
	if (bonusType == +MovingTileTypes::BombBreakerObject)
	{
		crushBombBreaker(cell);
	}
	else if(bonusType == +MovingTileTypes::ColumnBreakerObject)
	{
		crushColumnBreaker(cell);
	}
	else if (bonusType == +MovingTileTypes::RowBreakerObject)
	{
		crushRowBreaker(cell);
	}
	else if (bonusType == +MovingTileTypes::XBreakerObject)
	{
		crushXBreaker(cell);
	}
}

Cell* BoardController::findSeekerTarget(CellsList* targetsList) const
{
	auto targetCell = boardModel->getSeekerTarget();
	if(targetCell != nullptr)
	{
		return targetCell;
	}

	auto specialTiles = boardModel->getSpecialTiles();
	auto breakers = static_cast<__Array*>(specialTiles->objectForKey("breakers"));
	auto wafflePath = static_cast<__Array*>(specialTiles->objectForKey("wafflePath"));
	auto liquids = static_cast<__Array*>(specialTiles->objectForKey("liquids"));

	if (liquids->count() > 0)
	{
		targetCell = static_cast<Cell*>(liquids->getRandomObject());
		if (!Utils::containsCell(targetsList, targetCell))
		{
			return targetCell;
		}
	}
	if (wafflePath->count() > 0)
	{
		targetCell = static_cast<Cell*>(wafflePath->getRandomObject());
		if (!Utils::containsCell(targetsList, targetCell))
		{
			return targetCell;
		}
	}
	if (breakers->count() > 0)
	{
		targetCell = static_cast<Cell*>(breakers->getRandomObject());
		if (!Utils::containsCell(targetsList, targetCell))
		{
			return targetCell;
		}
	}
	return boardModel->getRandomCell();
}

void BoardController::update(float delta)
{
	Layer::update(delta);

}
