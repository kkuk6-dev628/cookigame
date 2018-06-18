#pragma once
#include "BoardController.h"
class DigDownController :
	public BoardController
{
public:
	DigDownController();
	~DigDownController();

	CREATE_FUNC(DigDownController)

	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:
	void addCellToBoard(char col, char row) override;
	void initDigDownGame();
	void crushCell(Cell* cell, bool forceClear = false) override;
	Cell* findSeekerTarget(CellsList* targetsList) const override;

	void showDigdownCollectingEffect(Cell* cell);
};

