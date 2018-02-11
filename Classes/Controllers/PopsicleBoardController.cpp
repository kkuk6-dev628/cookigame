#include "PopsicleBoardController.h"



PopsicleBoardController::PopsicleBoardController()
{
}


PopsicleBoardController::~PopsicleBoardController()
{
}

void PopsicleBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void PopsicleBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
	initPopsicleGame();
}

void PopsicleBoardController::initPopsicleGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_4")->setVisible(true);
	//objectCountNode->setString(StringUtils::toString(totalWaffleCount));
}


void PopsicleBoardController::addCellToBoard(char col, char row)
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
