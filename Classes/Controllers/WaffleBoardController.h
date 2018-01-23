#pragma once
#include "BoardController.h"

class WaffleBoardController : public BoardController
{
public:
	WaffleBoardController();
	~WaffleBoardController();

	CREATE_FUNC(WaffleBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

