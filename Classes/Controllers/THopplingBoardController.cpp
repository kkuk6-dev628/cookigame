#include "THopplingBoardController.h"
#include "ActionController.h"
#include "Models/Tiles/ThopplerTile.h"


THopplingBoardController::THopplingBoardController()
{
	thoplerTargets = new CellsList;
}


THopplingBoardController::~THopplingBoardController()
{
	delete thoplerTargets;
}

void THopplingBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void THopplingBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
	initThopplingGame();
}

void THopplingBoardController::initThopplingGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	if(getGoalType() == +GoalTypes::HopplingObject)
	{
		circleGroupNode->getChildByName("FileNode_2")->setVisible(true);
	}
	else
	{
		circleGroupNode->getChildByName("FileNode_5")->setVisible(true);
	}
	objectCountNode->setString(StringUtils::toString(totalObjectCount));
}


void THopplingBoardController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	auto cell = getMatchCell(col, row);
	if(cell == nullptr || cell->isOutCell)
	{
		return;
	}


	auto sourceTile = cell->getSourceTile();
	if (sourceTile != nullptr && (sourceTile->getType() == CRACKEROBJECT || sourceTile->getType() == PRETZELOBJECT))
	{
		if(crackerCells == nullptr)
		{
			crackerCells = new std::vector<Cell*>;
		}
		auto thopplerTile = cell->getTileAtLayer(LayerId::Toppling);
		if(thopplerTile != nullptr)
		{
			totalObjectCount ++;
		}
		else
		{
			crackerCells->push_back(cell);
		}
	}
}

Cell* THopplingBoardController::findSeekerTarget(CellsList* targetsList) const
{
	auto specialTiles = boardModel->getSpecialTiles();
	auto breakers = static_cast<__Array*>(specialTiles->objectForKey(BREAKERS));
	auto wafflePath = static_cast<__Array*>(specialTiles->objectForKey(WAFFLEPATH));
	auto liquids = static_cast<__Array*>(specialTiles->objectForKey(LIQUIDS));
	auto thopplers = static_cast<__Array*>(specialTiles->objectForKey(THOPPLERS));

	if(thopplers->count() > 0)
	{
		auto targetCell = static_cast<Cell*>(thopplers->getRandomObject());
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
	if (liquids->count() > 0)
	{
		auto targetCell = static_cast<Cell*>(liquids->getRandomObject());
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


Cell* THopplingBoardController::findHopplerTarget()
{
	if(crackerCells->size() > 0)
	{
		Cell* target = nullptr;
		do {
			auto random = rand_0_1() * crackerCells->size();
			target = crackerCells->at(random);
		} while (std::find(thoplerTargets->begin(), thoplerTargets->end(), target) != thoplerTargets->end());

		thoplerTargets->push_back(target);
		return target;
	}
	return nullptr;
}

void THopplingBoardController::crushCell(Cell* cell, bool forceClear)
{
	if (cell == nullptr || cell->isEmpty || cell->getSourceTile() == nullptr)
	{
		return;
	}
	auto sourceTile = cell->getSourceTile();
	if(sourceTile->getType() == CRACKEROBJECT || sourceTile->getType() == PRETZELOBJECT)
	{
		auto crushResult = cell->crushCell(true);
		auto thopplerTile = cell->getTileAtLayer(LayerId::Toppling);
		if(thopplerTile != nullptr)
		{
			if(crushResult)
			{
				static_cast<ThopplerTile*>(thopplerTile)->moveDown();
				cell->setSourceTile(thopplerTile);
				cell->removeTileAtLayer(LayerId::Toppling);
			}
			else
			{
				if(pendingThopplers == nullptr)
				{
					pendingThopplers = new std::vector<Cell*>;
				}
				auto findResult = std::find(pendingThopplers->begin(), pendingThopplers->end(), cell);
				if(!cell->dirty && findResult == pendingThopplers->end())
				{
					cell->dirty = true;
					pendingThopplers->push_back(cell);
				}
			}
		}
		else if(crushResult)
		{
			crackerCells->erase(std::remove(crackerCells->begin(), crackerCells->end(), cell), crackerCells->end());
		}
	}
	else if(sourceTile->getType() == TOPPLINGOBJECT || sourceTile->getType() == HOPPLINGOBJECT)
	{
		showThopplerCollectingEffect(cell, sourceTile->getType());
		cell->crushCell(true);
	}
	else
	{
		BoardController::crushCell(cell, forceClear);
	}
}

void THopplingBoardController::processCustomLogic(float dt)
{
	movePendingThopplers();
}

void THopplingBoardController::movePendingThopplers()
{
	if (fallingTileCount > 0 || gameState != GameState::Idle)
	{
		return;
	}

	if(pendingThopplers == nullptr || pendingThopplers->size() == 0)
	{
		return;
	}

	for (auto cell : *pendingThopplers)
	{
		auto thoppler = cell->getTileAtLayer(LayerId::Toppling);
		if (thoppler == nullptr)
		{
			continue;
		}
		auto typeString = thoppler->getType();

		auto sourceTile = cell->getSourceTile();
		if (sourceTile->getType() != CRACKEROBJECT && sourceTile->getType() != PRETZELOBJECT)
		{
			static_cast<ThopplerTile*>(thoppler)->moveDown();
			cell->setSourceTile(thoppler);
			cell->removeTileAtLayer(LayerId::Toppling);
		}
		if (strcmp(typeString.c_str(), TOPPLINGOBJECT) == 0)
		{
			showTopplerMoveEffect(cell);
		}
		else if (strcmp(typeString.c_str(), HOPPLINGOBJECT) == 0)
		{
			showHopplerMoveEffect(cell);
		}

	}
	pendingThopplers->clear();
	thoplerTargets->clear();
}

void THopplingBoardController::showTopplerMoveEffect(Cell* cell)
{
	auto hopplerTile = cell->getTileAtLayer(LayerId::Toppling);

	auto hopplingPath = findTopplingTarget(cell);
	//while (std::find(thoplerTargets->begin(), thoplerTargets->end(), hopplingPath->back()) != thoplerTargets->end())
	//{
	//	hopplingPath = findTopplingTarget(cell);
	//}
	if(hopplingPath == nullptr || hopplingPath->size() < 2)
	{
		return;
	}

	SoundEffects topplingEffect = SoundEffects::VO_toppler_move_4;
	switch (hopplingPath->size())
	{
	case 1:
		topplingEffect = SoundEffects::VO_toppler_move_1;
		break;
	case 2:
		topplingEffect = SoundEffects::VO_toppler_move_2;
		break;
	case 3:
		topplingEffect = SoundEffects::VO_toppler_move_3;
		break;
	default:
		topplingEffect = SoundEffects::VO_toppler_move_4;
		break;
	}
	soundController->playEffectSound(topplingEffect);

	auto hopplerShow = poolController->getTopplerShow();
	hopplerShow->setPosition(cell->getBoardPos());
	showObjectsLayer->addChild(hopplerShow);
	hopplerTile->setVisible(false);

	Cell* targetCell = hopplingPath->back();
	if (targetCell->dirty)
	{
		hopplingPath->pop_back();
		targetCell = hopplingPath->back();
	}
	crackerCells->erase(std::remove(crackerCells->begin(), crackerCells->end(), targetCell), crackerCells->end());
	crackerCells->push_back(cell);
	cell->removeTileAtLayer(LayerId::Toppling);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(hopplerShow);
	ckAction.action = actionController->createTopplerMoveAction(hopplingPath, [=] {
		PoolController::getInstance()->recycleTopplerShow(hopplerShow);
		hopplerTile->setVisible(true);
		targetCell->setTileToLayer(hopplerTile, LayerId::Toppling);
		targetCell->dirty = false;
		hopplerTile->setPosition(targetCell->getBoardPos());
	}, ckAction.node);
	actionController->pushAction(ckAction, true);
	CC_SAFE_DELETE(hopplingPath);
}

void THopplingBoardController::showHopplerMoveEffect(Cell* cell)
{
	auto topplerTile = cell->getTileAtLayer(LayerId::Toppling);

	auto targetCell = findHopplerTarget();
	if (targetCell == nullptr)
	{
		return;
	}

	auto topplerShow = poolController->getHopplerShow();
	topplerShow->setPosition(cell->getBoardPos());
	showObjectsLayer->addChild(topplerShow);
	topplerTile->setVisible(false);

	crackerCells->erase(std::remove(crackerCells->begin(), crackerCells->end(), targetCell), crackerCells->end());
	crackerCells->push_back(cell);
	cell->removeTileAtLayer(LayerId::Toppling);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(topplerShow);
	ckAction.action = actionController->createJumpAction(ckAction.node, targetCell->getBoardPos(), 2 * CellSize, [=] {
		PoolController::getInstance()->recycleHopplerShow(topplerShow);
		topplerTile->setVisible(true);
		targetCell->setTileToLayer(topplerTile, LayerId::Toppling);
		targetCell->dirty = false;
		topplerTile->setPosition(targetCell->getBoardPos());
	});
	actionController->pushAction(ckAction, true);
}

void THopplingBoardController::showThopplerCollectingEffect(Cell* startCell, std::string type)
{
	SpriteShowObject* topplerShow;
	if (type == TOPPLINGOBJECT)
	{
		topplerShow = poolController->getTopplerShow();
	}
	else
	{
		topplerShow = poolController->getHopplerShow();
	}
	topplerShow->setPosition(startCell->getBoardPos());
	effectNode->addChild(topplerShow);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(topplerShow);
	ckAction.action = actionController->createJumpAction(ckAction.node, objectTargetPos, 2 * CellSize, [=] {
		if (type == TOPPLINGOBJECT)
		{
			this->poolController->recycleTopplerShow(topplerShow);
		}
		else
		{
			this->poolController->recycleHopplerShow(topplerShow);
		}
		this->increaseObjectCount();
	});
	actionController->pushAction(ckAction, true);
}

CellsList* THopplingBoardController::findTopplingTarget(Cell* cell)
{
	char* oldIndent = nullptr;
	char* newIndent = nullptr;
	Cell* oldCell = cell;
	Cell* newCell = cell;
	auto hopplingPath = new CellsList;
	char loopCount = 0;
	do
	{
		oldCell = newCell;
		newCell = findNextCrackerCell(oldCell, oldIndent, &newIndent);
		if(oldIndent != newIndent)
		{
			hopplingPath->push_back(oldCell);
		}
		oldIndent = newIndent;
		loopCount++;
	} while (newCell != nullptr && loopCount < 5);

	if (oldCell != nullptr && !oldCell->dirty && std::find(thoplerTargets->begin(), thoplerTargets->end(), oldCell) == thoplerTargets->end())
	{
		hopplingPath->push_back(oldCell);
	}
	return hopplingPath;
}

Cell* THopplingBoardController::findNextCrackerCell(Cell* cell, char* inIndent, char** outIndent)
{
	for(auto indent : AdjacentIndents)
	{
		auto adjCell = getMatchCell(cell->gridPos.Col + indent[1], cell->gridPos.Row + indent[0]);
		if(adjCell == nullptr || adjCell->isOutCell || adjCell->isEmpty || adjCell->containsThoppler()){continue;}
		auto movingTile = adjCell->getSourceTile();
		if((movingTile->getType() == CRACKEROBJECT || movingTile->getType() == PRETZELOBJECT) && !(inIndent != nullptr && inIndent[0] * -1 == indent[0] && inIndent[1] * -1 == indent[1]))
		{
			*outIndent = indent;
			return adjCell;
		}
	}
	return nullptr;
}

