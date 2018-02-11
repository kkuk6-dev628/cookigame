#pragma once
#include "BoardController.h"
#include "THopplingBoardController.h"

class HopplingBoardController : public THopplingBoardController
{
public:
	HopplingBoardController();
	~HopplingBoardController();

	CREATE_FUNC(HopplingBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

