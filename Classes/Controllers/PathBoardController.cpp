#include "PathBoardController.h"



PathBoardController::PathBoardController()
{
}


PathBoardController::~PathBoardController()
{
}

void PathBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void PathBoardController::initWithNode(Node* rootNode)
{
	BoardController::initWithNode(rootNode);
	initWaffleGame();
}

void PathBoardController::initWaffleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_3")->setVisible(true);
	objectCountNode->setVisible(false);
}


void PathBoardController::addCellToBoard(char col, char row)
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
