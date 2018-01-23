#include "THopplingBoardController.h"



THopplingBoardController::THopplingBoardController()
{
}


THopplingBoardController::~THopplingBoardController()
{
}

void THopplingBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void THopplingBoardController::initWithNode(Node* rootNode)
{
	BoardController::initWithNode(rootNode);
	initWaffleGame();
}

void THopplingBoardController::initWaffleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_2")->setVisible(true);
	//objectCountNode->setString(StringUtils::toString(totalWaffleCount));
}


void THopplingBoardController::addCellToBoard(char col, char row)
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
