#include "PathBoardController.h"
#include "ActionController.h"


PathBoardController::PathBoardController()
{
}


PathBoardController::~PathBoardController()
{
}

void PathBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void PathBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
	initPathGame();
}

void PathBoardController::initPathGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_3")->setVisible(true);
	objectCountNode->setVisible(false);
}

void PathBoardController::pathMoverCollected(Cell* cell)
{
	auto powerShow = poolController->getPowerShow();
	powerShow->setPosition(cell->getBoardPos());
	showObjectsLayer->addChild(powerShow);
	flyingPowersCount++;
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(powerShow);
	ckAction.action = actionController->createJumpAction(ckAction.node, pathFollowerObject->getPosition(), 2 * CellSize, [=]()
                                                              {
	                                                              PoolController::getInstance()->recyclePowerShow(powerShow);
	                                                              collectedPowerCount++;
	                                                              flyingPowersCount--;
                                                              });
	actionController->pushAction(ckAction, false);

}


void PathBoardController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	auto cell = getMatchCell(col, row);

	auto movingTile = cell->getSourceTile();
	if(movingTile != nullptr)
	{
		if (strcmp(movingTile->getType().c_str(), "PathFollowerObject") == 0)
		{
			pathFollowerObject = movingTile;
			pathFollowerShow = Sprite::create();
			auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("PathFollower.png");
			pathFollowerShow->setSpriteFrame(spriteFrame);
			pathFollowerShow->setPosition(pathFollowerObject->getPosition());
			pathFollowerShow->setAnchorPoint(pathFollowerObject->getAnchorPoint());
			showObjectsLayer->addChild(pathFollowerShow);
			pathFollowerShow->setVisible(false);
		}
		else if (strcmp(movingTile->getType().c_str(), "PathGoalObject") == 0)
		{
			pathGoalCell = cell;
		}
	}
	//if(cell == nullptr || cell->isOutCell)
	//{
	//	return;
	//}

	//auto waffleTile = cell->getTileAtLayer(LayerId::Waffle);
	//if(waffleTile != nullptr)
	//{
	//	totalWaffleCount += waffleTile->getLayers();
	//}
}

void PathBoardController::processCustomLogic(float dt)
{
	movePathFollower();

}

CellsList* PathBoardController::findPath()
{
	auto movePath = new CellsList;
	Cell* pathCell = pathFollowerObject->getCell();
	while(collectedPowerCount > 0)
	{
		pathCell = getNextPath(pathCell);
		if(pathCell->canMoveCream())
		{
			movePath->push_back(pathCell);
			collectedPowerCount--;
		}
		else
		{
			break;
		}
	}
	return movePath;
}

Cell* PathBoardController::getNextPath(Cell* cell)
{
	auto pathObject = cell->getTileAtLayer(LayerId::PathConveyor);
	if (pathObject == nullptr)
	{
		return nullptr;
	}
	auto pathString = pathObject->getDirectionString();
	auto nextPathIndent = MoveDirectionsMap.at(pathString);
	char col = cell->gridPos.Col + nextPathIndent[1];
	char row = cell->gridPos.Row + nextPathIndent[0];
	return getMatchCell(col, row);
}

void PathBoardController::movePathFollower()
{
	if (flyingPowersCount > 0 || fallingTileCount > 0 || gameState != Idle || collectedPowerCount <= 0)
	{
		return;
	}

	auto movePath = findPath();
	auto moveToCell = pathFollowerObject->getCell();
	Cell* lastCell = nullptr;
	if(movePath->size() > 0)
	{
		for(auto cell : *movePath)
		{
			auto movingTile = cell->getMovingTile();
			if(movingTile != nullptr)
			{
				movingTile->showMoveAction(moveToCell);
				moveToCell->setSourceTile(movingTile);
			}
			else
			{
				moveToCell->isEmpty = true;
			}
			moveToCell = cell;
			lastCell = cell;
		}
		showPathFollowerMovingAction(movePath);

		lastCell->setSourceTile(pathFollowerObject);
		pathFollowerObject->setPosition(lastCell->getBoardPos());
	}
	else
	{
		auto nextCell = getNextPath(pathFollowerObject->getCell());
		if(nextCell == pathGoalCell)
		{
			showPathFollowerMovingAction(pathGoalCell);
			collectedPowerCount = 0;
			finishLevel();
		}
		else if(nextCell != nullptr && !nextCell->canMove())
		{
			showFrontCrushAction(nextCell);
		}
	}
}

void PathBoardController::showFrontCrushAction(Cell* frontCell)
{
	auto fixTileLayers = frontCell->getSourceTile()->getLayers();
	auto crushCount = fixTileLayers > collectedPowerCount ? collectedPowerCount : fixTileLayers;
	collectedPowerCount -= crushCount;
	pathFollowerShow->setPosition(pathFollowerObject->getPosition());
	pathFollowerShow->setVisible(true);
	pathFollowerObject->setVisible(false);
	fallingTileCount++;
	CKAction ckAction;
	ckAction.node = pathFollowerShow;
	ckAction.action = actionController->createFrontCrushAction(pathFollowerShow, frontCell->getBoardPos(), [=] {
		frontCell->crushCell(true);
	}, [=] {
		pathFollowerObject->setVisible(true);
		pathFollowerShow->setVisible(false);
		fallingTileCount--;
	}, crushCount);

	actionController->pushAction(ckAction, false);
}

Cell* PathBoardController::findSeekerTarget(CellsList* targetsList) const
{
	auto specialTiles = boardModel->getSpecialTiles();
	auto breakers = static_cast<__Array*>(specialTiles->objectForKey(BREAKERS));
	auto wafflePath = static_cast<__Array*>(specialTiles->objectForKey(WAFFLEPATH));
	auto liquids = static_cast<__Array*>(specialTiles->objectForKey(LIQUIDS));

	if (liquids->count() > 0)
	{
		auto targetCell = static_cast<Cell*>(liquids->getRandomObject());
		if (!Utils::containsCell(targetsList, targetCell))
		{
			return targetCell;
		}
	}
	if (wafflePath->count() > 0)
	{
		auto targetCell = static_cast<Cell*>(wafflePath->getRandomObject());
		if (!Utils::containsCell(targetsList, targetCell))
		{
			return targetCell;
		}
	}
	if (breakers->count() > 0)
	{
		auto targetCell = static_cast<Cell*>(breakers->getRandomObject());
		if (!Utils::containsCell(targetsList, targetCell))
		{
			return targetCell;
		}
	}
	return boardModel->getRandomCell();
}

void PathBoardController::showPathFollowerMovingAction(Cell* cell)
{
	pathFollowerShow->setVisible(true);
	pathFollowerObject->setVisible(false);
	gameState = SwappingTile;
	CKAction ckAction;
	ckAction.node = pathFollowerShow;
	ckAction.action = actionController->createTileMoveAction(pathFollowerShow->getPosition(), cell->getBoardPos(), [=] {
		pathFollowerShow->setVisible(false);
		pathFollowerObject->setVisible(true);
		gameState = Idle;
	}, ckAction.node);

	actionController->pushAction(ckAction, false);
}

void PathBoardController::showPathFollowerMovingAction(CellsList* path)
{
	pathFollowerShow->setVisible(true);
	pathFollowerObject->setVisible(false);
	gameState = SwappingTile;
	CKAction ckAction;
	ckAction.node = pathFollowerShow;
	ckAction.action = actionController->createPathFollowerMoveAction(path, [=] {
		pathFollowerShow->setVisible(false);
		pathFollowerObject->setVisible(true);
		gameState = Idle;
	}, ckAction.node);

	actionController->pushAction(ckAction, false);
}

void PathBoardController::spawnNewTile(Cell* cell)
{
	if(boardModel->getPathMoversCount() > 2)
	{
		BoardController::spawnNewTile(cell);
	}
	else
	{
		cell->spawnPathMover();
	}
}


