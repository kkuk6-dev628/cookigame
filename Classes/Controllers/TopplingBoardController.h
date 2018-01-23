#pragma once
#include "BoardController.h"

class TopplingBoardController : public BoardController
{
public:
	TopplingBoardController();
	~TopplingBoardController();

	CREATE_FUNC(TopplingBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

