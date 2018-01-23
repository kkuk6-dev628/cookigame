#include "HiderBoardController.h"



HiderBoardController::HiderBoardController()
{
}


HiderBoardController::~HiderBoardController()
{
}

void HiderBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void HiderBoardController::initWithNode(Node* rootNode)
{
	BoardController::initWithNode(rootNode);
	initWaffleGame();
}

void HiderBoardController::initWaffleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_1")->setVisible(true);
	//objectCountNode->setString(StringUtils::toString(totalWaffleCount));
}


void HiderBoardController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	//auto cell = getMatchCell(col, row);
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
