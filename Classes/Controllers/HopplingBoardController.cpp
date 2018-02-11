#include "HopplingBoardController.h"


HopplingBoardController::HopplingBoardController()
{
}


HopplingBoardController::~HopplingBoardController()
{
}

void HopplingBoardController::initWithModel(BoardModel* model)
{
	THopplingBoardController::initWithModel(model);
}

void HopplingBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	THopplingBoardController::initWithNode(rootNode, effectNode);
}


void HopplingBoardController::addCellToBoard(char col, char row)
{
	THopplingBoardController::addCellToBoard(col, row);
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
