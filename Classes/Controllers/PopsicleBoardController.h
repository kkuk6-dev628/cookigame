#pragma once
#include "BoardController.h"

class PopsicleBoardController : public BoardController
{
public:
	PopsicleBoardController();
	~PopsicleBoardController();

	CREATE_FUNC(PopsicleBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initPopsicleGame();

private:
	short totalWaffleCount = 0;

};

