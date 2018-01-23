#pragma once
#include "BoardController.h"

class HopplingBoardController : public BoardController
{
public:
	HopplingBoardController();
	~HopplingBoardController();

	CREATE_FUNC(HopplingBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

