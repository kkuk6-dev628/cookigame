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

	waffleTargetPos = Utils::convertPos(circleGroupNode, effectNode);
	objectCountNode->setString(StringUtils::toString(totalWaffleCount));
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
	ckAction.action = actionController->createJumpAction(ckAction.node, waffleTargetPos, 2 * CellSize, [=]()
	{
		PoolController::getInstance()->recycleWaffleShow(waffleShow);
		collectedWaffleCount++;
	});
	effectNode->addChild(waffleShow);
	actionController->pushAction(ckAction, false);
}


void WaffleBoardController::collectWaffle()
{
	collectedWaffleCount++;
	objectCountNode->setString(StringUtils::toString(totalWaffleCount - collectedWaffleCount));
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
	if(waffleTile != nullptr)
	{
		totalWaffleCount += waffleTile->getLayers();
	}
}

void WaffleBoardController::checkObjective()
{
	if (totalWaffleCount == collectedWaffleCount)
	{
		gameState = GameState::Completed;
		showGameWinDlg();
	}
}