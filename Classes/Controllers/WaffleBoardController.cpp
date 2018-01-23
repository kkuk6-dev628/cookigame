#include "WaffleBoardController.h"



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

void WaffleBoardController::initWithNode(Node* rootNode)
{
	BoardController::initWithNode(rootNode);
	initWaffleGame();
}

void WaffleBoardController::initWaffleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_6")->setVisible(true);
	objectCountNode->setString(StringUtils::toString(totalWaffleCount));
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
