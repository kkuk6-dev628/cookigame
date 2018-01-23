#include "HopplingBoardController.h"



HopplingBoardController::HopplingBoardController()
{
}


HopplingBoardController::~HopplingBoardController()
{
}

void HopplingBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void HopplingBoardController::initWithNode(Node* rootNode)
{
	BoardController::initWithNode(rootNode);
	initWaffleGame();
}

void HopplingBoardController::initWaffleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_5")->setVisible(true);
	//objectCountNode->setString(StringUtils::toString(totalWaffleCount));
}


void HopplingBoardController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	auto cell = getMatchCell(col, row);
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
