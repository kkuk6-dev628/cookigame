#pragma once
#include "BoardController.h"

class PopsicleBoardController : public BoardController
{
public:
	PopsicleBoardController();
	~PopsicleBoardController();

	CREATE_FUNC(PopsicleBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initPopsicleGame();
	void crushUnderCells(Cell* cell) override;
	void processCustomLogic(float dt) override;
	Cell* findSeekerTarget(CellsList* targetsList) const override;
	CellsList* getSeekerTargets(int count) const override;

private:
	std::vector<Cell*>* popsicleCells = nullptr;

	std::vector<Cell*>* coveredPopsicleCells = nullptr;

	void checkPopsicles();
	void canCollectedPopsicle(Cell* cell);
	void showPopsicleCollectingAction(Cell* cell);
	void showPopNormalCollectingAction(Cell* cell);
	void showPopLineCollectingAction(Cell* cell);
	void showPopBombCollectingAction(Cell* cell);
	void showPopRainbowCollectingAction(Cell* cell);

};

