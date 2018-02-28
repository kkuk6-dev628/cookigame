#include "PopsicleBoardController.h"
#include "Models/Tiles/PopsicleObject.h"
#include "ActionController.h"


PopsicleBoardController::PopsicleBoardController()
{
}


PopsicleBoardController::~PopsicleBoardController()
{
}

void PopsicleBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void PopsicleBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
	initPopsicleGame();
}

void PopsicleBoardController::initPopsicleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_4")->setVisible(true);
	objectCountNode->setString(StringUtils::toString(totalObjectCount));
}

void PopsicleBoardController::crushUnderCells(Cell* cell)
{
	if (cell->crushUnderTiles(LayerId::Cover))
	{
		//canCollectedPopsicle(cell);
	}
}


void PopsicleBoardController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	auto cell = getMatchCell(col, row);
	if(cell == nullptr || cell->isOutCell)
	{
		return;
	}

	auto popsicle = static_cast<PopsicleObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if(popsicle != nullptr)
	{
		totalObjectCount ++;
		if(popsicleCells == nullptr)
		{
			popsicleCells = new std::vector<Cell*>;
		}
		popsicleCells->push_back(cell);

		if(coveredPopsicleCells == nullptr)
		{
			coveredPopsicleCells = new std::vector<Cell*>;
		}
		for (char i = 0; i < popsicle->getColSpan(); i++)
		{
			for (char j = 0; j < popsicle->getRowSpan(); j++)
			{
				auto checkCell = getMatchCell(cell->gridPos.Col + i, cell->gridPos.Row - j);
				if (checkCell == nullptr || checkCell->isOutCell || checkCell->isEmpty)
				{
					continue;
				}
				coveredPopsicleCells->push_back(checkCell);
			}
		}
	}
}

void PopsicleBoardController::processCustomLogic(float dt)
{
	checkPopsicles();
}

void PopsicleBoardController::checkPopsicles()
{
	if (fallingTileCount > 0 || gameState != Idle) return;

	for (auto cell : *popsicleCells)
	{
		canCollectedPopsicle(cell);
	}
}

void PopsicleBoardController::canCollectedPopsicle(Cell* cell)
{
	auto popsicle = static_cast<PopsicleObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if (popsicle == nullptr)
	{
		return;
	}
	auto allStripped = true;
	for (char i = 0; i < popsicle->getColSpan(); i++)
	{
		for (char j = 0; j < popsicle->getRowSpan(); j++)
		{
			auto checkCell = getMatchCell(cell->gridPos.Col + i, cell->gridPos.Row - j);
			if (checkCell == nullptr || checkCell->isOutCell || checkCell->isEmpty)
			{
				continue;
			}
			if (checkCell->containsIceCover())
			{
				allStripped = false;
				//break;
			}
			else
			{
				coveredPopsicleCells->erase(std::remove(coveredPopsicleCells->begin(), coveredPopsicleCells->end(), checkCell), coveredPopsicleCells->end());
			}
		}
		//if (allStripped == false) break;
	}

	if (allStripped)
	{
		showPopsicleCollectingAction(cell);
		//popsicleCells->erase(std::remove(popsicleCells->begin(), popsicleCells->end(), cell), popsicleCells->end());

	}
}

Cell* PopsicleBoardController::findSeekerTarget(std::list<Cell*>* targetsList) const
{
	if(coveredPopsicleCells != nullptr && coveredPopsicleCells->size() > 0)
	{
		Cell* retCell = nullptr;
		auto loopCount = 3;
		do
		{
			retCell = coveredPopsicleCells->at(coveredPopsicleCells->size() * rand_0_1());
			loopCount--;
		} 
		while (Utils::containsCell(targetsList, retCell) && loopCount > 0);
		if(!Utils::containsCell(targetsList, retCell))
		{
			return retCell;
		}
	}
	return BoardController::findSeekerTarget(targetsList);
}

void PopsicleBoardController::showPopsicleCollectingAction(Cell* cell)
{
	auto popsicle = static_cast<PopsicleObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if(popsicle == nullptr)
	{
		return;
	}
	switch (popsicle->getPopType())
	{
	case PopsicleType::line:
		showPopLineCollectingAction(cell);
		break;
	case PopsicleType::bomb:
		showPopBombCollectingAction(cell);
		break;
	case PopsicleType::rainbow:
		showPopRainbowCollectingAction(cell);
		break;
	default:
		showPopNormalCollectingAction(cell);
		break;
	}
	popsicle->removeFromParentAndCleanup(true);
	cell->removeTileAtLayer(LayerId::UnderCover);
}

void PopsicleBoardController::showPopNormalCollectingAction(Cell* cell)
{
	auto popsicle = static_cast<PopsicleObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if (popsicle == nullptr)
	{
		return;
	}
	auto popsicleShow = poolController->getPopsicleShow();
	popsicleShow->setPosition(cell->getBoardPos());
	effectNode->addChild(popsicleShow);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(popsicleShow);
	ckAction.action = actionController->createPopCollectionAction(objectTargetPos, [=] {
		this->poolController->recyclePopsicleShow(popsicleShow);
		this->increaseObjectCount();
	}, ckAction.node);
	actionController->pushAction(ckAction, true);

}

void PopsicleBoardController::showPopLineCollectingAction(Cell* cell)
{
	auto popsicle = static_cast<PopsicleObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if (popsicle == nullptr)
	{
		return;
	}
	auto sp = popsicle->getSpriteFrame();
	auto popsicleShow = poolController->getPopLineShow();
	popsicleShow->setSpriteFrame(sp);


	auto cellPos = cell->getBoardPos();
	popsicleShow->setPosition(cellPos);
	effectNode->addChild(popsicleShow);
	auto targetPos = popsicle->getLineTarget(boardModel->getWidth() * CellSize, boardModel->getHeight() * CellSize);
	CKAction ckAction_0;
	ckAction_0.node = reinterpret_cast<Node*>(popsicleShow);
	ckAction_0.action = actionController->createPopLineAction(targetPos, [=]{

		CKAction ckAction;
		ckAction.node = reinterpret_cast<Node*>(popsicleShow);
		ckAction.action = actionController->createPopCollectionAction(objectTargetPos, [=] {
			this->poolController->recyclePopLineShow(popsicleShow);
			this->increaseObjectCount();
		}, ckAction.node);
		this->actionController->pushAction(ckAction, true);
	}, popsicleShow);
	actionController->pushAction(ckAction_0, true);

	if(popsicle->getColSpan() > popsicle->getRowSpan())
	{
		crushRowBreaker(cell, false);
	}
	else
	{
		crushColumnBreaker(cell, false);
	}
}

void PopsicleBoardController::showPopBombCollectingAction(Cell* cell)
{
	auto popsicle = static_cast<PopsicleObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if (popsicle == nullptr)
	{
		return;
	}
	auto popBombShow = poolController->getPopBombShow();
	popBombShow->setPosition(cell->getBoardPos());
	effectNode->addChild(popBombShow);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(popBombShow);
	ckAction.action = actionController->createPopBombAction(objectTargetPos, [=] {
		this->poolController->recyclePopBombShow(popBombShow);
		this->increaseObjectCount();
	}, ckAction.node);
	actionController->pushAction(ckAction, true);
	crushBombBreaker(cell);
}

void PopsicleBoardController::showPopRainbowCollectingAction(Cell* cell)
{
	auto popsicle = static_cast<PopsicleObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if (popsicle == nullptr)
	{
		return;
	}
	auto popBombShow = poolController->getPopRainbowShow();
	popBombShow->setPosition(cell->getBoardPos());
	effectNode->addChild(popBombShow);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(popBombShow);
	ckAction.action = actionController->createPopBombAction(objectTargetPos, [=] {
		this->poolController->recyclePopRainbowShow(popBombShow);
		this->increaseObjectCount();
	}, ckAction.node);
	actionController->pushAction(ckAction, true);
	auto match = Match::create();
	match->refCell = cell;
	match->color = cell->getMovingTile()->getTileColor();
	crushRainbowMatch(match);
}
