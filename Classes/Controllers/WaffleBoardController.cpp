#include "WaffleBoardController.h"
#include "ActionController.h"


WaffleBoardController::WaffleBoardController()
{
}


WaffleBoardController::~WaffleBoardController()
{
}

void WaffleBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void WaffleBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
	initWaffleGame();
}

void WaffleBoardController::initWaffleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_6")->setVisible(true);

	objectCountNode->setString(StringUtils::toString(totalObjectCount));
}

void WaffleBoardController::crushUnderCells(Cell* cell)
{
	if(cell->crushUnderTiles(LayerId::Waffle))
	{
		showWaffleCollectAction(cell);
	}
}

void WaffleBoardController::showWaffleCollectAction(Cell* cell)
{
	auto waffleShow = poolController->getWaffleShow();
	waffleShow->setPosition(cell->getBoardPos());
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(waffleShow);
	ckAction.action = actionController->createJumpAction(ckAction.node, objectTargetPos, 2 * CellSize, [=]()
	{
		PoolController::getInstance()->recycleWaffleShow(waffleShow);
		this->increaseObjectCount();
	});
	effectNode->addChild(waffleShow);
	actionController->pushAction(ckAction, false);
}

void WaffleBoardController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	auto cell = getMatchCell(col, row);
	if(cell == nullptr || cell->isOutCell)
	{
		return;
	}

	auto waffleTile = cell->getTileAtLayer(LayerId::Waffle);
	if(waffleTile != nullptr && waffleTile->getType() == WAFFLEOBJECT)
	{
		totalObjectCount += waffleTile->getLayers();
	}
}
