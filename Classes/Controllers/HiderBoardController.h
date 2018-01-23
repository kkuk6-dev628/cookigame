#pragma once
#include "BoardController.h"

class HiderBoardController : public BoardController
{
public:
	HiderBoardController();
	~HiderBoardController();

	CREATE_FUNC(HiderBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

