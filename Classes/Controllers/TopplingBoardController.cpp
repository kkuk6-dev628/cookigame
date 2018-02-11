#include "TopplingBoardController.h"



TopplingBoardController::TopplingBoardController()
{
}


TopplingBoardController::~TopplingBoardController()
{
}

void TopplingBoardController::initWithModel(BoardModel* model)
{
	THopplingBoardController::initWithModel(model);
}

void TopplingBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	THopplingBoardController::initWithNode(rootNode, effectNode);
}


void TopplingBoardController::addCellToBoard(char col, char row)
{
	THopplingBoardController::addCellToBoard(col, row);
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
