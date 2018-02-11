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

void HiderBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
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
}
