#pragma once
#include "BoardController.h"

class PopsicleBoardController : public BoardController
{
public:
	PopsicleBoardController();
	~PopsicleBoardController();

	CREATE_FUNC(PopsicleBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

