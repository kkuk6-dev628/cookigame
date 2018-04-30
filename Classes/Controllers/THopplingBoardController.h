#pragma once
#include "BoardController.h"

class THopplingBoardController : public BoardController
{
public:
	THopplingBoardController();
	~THopplingBoardController();

	CREATE_FUNC(THopplingBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initThopplingGame();

	void crushCell(Cell* cell, bool forceClear = false) override;
	Cell* findThopplerTarget();
	CellsList* findHopplingTarget(Cell* cell);
	Cell* findNextCrackerCell(Cell* cell, char* inIndent, char** indent);

	void processCustomLogic(float dt) override;
	Cell* findSeekerTarget(CellsList* targetsList) const override;

	virtual void movePendingThopplers();
	void showHopplerMoveEffect(Cell* startCell);
	void showTopplerMoveEffect(Cell* cell);
	void showThopplerCollectingEffect(Cell* , std::string typeString);

private:
	std::vector<Cell*>* crackerCells = nullptr;
	std::vector<Cell*>* pendingThopplers = nullptr;
	CellsList* thoplerTargets = nullptr;
};

