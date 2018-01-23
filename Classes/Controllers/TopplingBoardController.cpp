#include "TopplingBoardController.h"



TopplingBoardController::TopplingBoardController()
{
}


TopplingBoardController::~TopplingBoardController()
{
}

void TopplingBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void TopplingBoardController::initWithNode(Node* rootNode)
{
	BoardController::initWithNode(rootNode);
	initWaffleGame();
}

void TopplingBoardController::initWaffleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_2")->setVisible(true);
	//objectCountNode->setString(StringUtils::toString(totalWaffleCount));
}


void TopplingBoardController::addCellToBoard(char col, char row)
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
