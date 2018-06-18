#include "DigDownController.h"
#include "ActionController.h"


DigDownController::DigDownController()
{
}


DigDownController::~DigDownController()
{
}

void DigDownController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
	initDigDownGame();
}

void DigDownController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	auto cell = getMatchCell(col, row);
	if (cell == nullptr || cell->isOutCell)
	{
		return;
	}

	auto sourceTile = cell->getSourceTile();
	if (sourceTile != nullptr && (sourceTile->getType() == DIGDOWNYUMBLEOBJECT))
	{
		totalObjectCount++;
	}
}

void DigDownController::initDigDownGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_7")->setVisible(true);
	objectCountNode->setString(StringUtils::toString(totalObjectCount));
}

void DigDownController::crushCell(Cell* cell, bool forceClear)
{
	if (cell == nullptr || cell->isEmpty || cell->getSourceTile() == nullptr)
	{
		return;
	}
	auto sourceTile = cell->getSourceTile();
	if (sourceTile->getType() == DIGDOWNYUMBLEOBJECT)
	{
		if(cell->crushCell(true))
		{
			showDigdownCollectingEffect(cell);
		}
	}
	else
	{
		BoardController::crushCell(cell, forceClear);
	}

}

Cell* DigDownController::findSeekerTarget(CellsList* targetsList) const
{
	auto specialTiles = boardModel->getSpecialTiles();
	auto breakers = static_cast<__Array*>(specialTiles->objectForKey(BREAKERS));
	auto liquids = static_cast<__Array*>(specialTiles->objectForKey(LIQUIDS));
	auto digdowns = static_cast<__Array*>(specialTiles->objectForKey(DIGDOWNS));

	if (digdowns->count() > 0)
	{
		auto targetCell = static_cast<Cell*>(digdowns->getRandomObject());
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
			//cocos2d::log("test value: %d", targetCell);
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


void DigDownController::showDigdownCollectingEffect(Cell* cell)
{
	auto digdownShow = poolController->getDigdownShow();
	digdownShow->setPosition(cell->getBoardPos());
	effectNode->addChild(digdownShow);
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(digdownShow);
	ckAction.action = actionController->createPopBombAction(objectTargetPos, [=] {
		this->poolController->recycleDigdownShow(digdownShow);
		this->increaseObjectCount();
	}, digdownShow);
	actionController->pushAction(ckAction, true);
}
