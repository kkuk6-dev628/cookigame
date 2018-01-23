#pragma once
#include "BoardController.h"

class THopplingBoardController : public BoardController
{
public:
	THopplingBoardController();
	~THopplingBoardController();

	CREATE_FUNC(THopplingBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

