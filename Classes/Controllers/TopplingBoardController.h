#pragma once
#include "BoardController.h"
#include "THopplingBoardController.h"

class TopplingBoardController : public THopplingBoardController
{
public:
	TopplingBoardController();
	~TopplingBoardController();

	CREATE_FUNC(TopplingBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	//void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

