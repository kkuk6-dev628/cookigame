#pragma once
#include "BoardController.h"
#include "Models/Tiles/HiderGroup.h"

class HiderBoardController : public BoardController
{
public:
	HiderBoardController();
	~HiderBoardController();

	CREATE_FUNC(HiderBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:
	void addCellToBoard(char col, char row) override;
	void crushUnderCells(Cell* cell) override;
	void crushCell(Cell* pCell) override;
	void processCustomLogic(float dt) override;
	Cell* findSeekerTarget(std::list<Cell*>* targetsList) const override;

	void initHiderGame();

private:
	std::map<std::string, HiderGroup*>* hidersMap = nullptr;
	HiderSegmentObject* findNextSegment(HiderSegmentObject* seg, HiderSegmentObject* pastSeg) const;
	std::list<Cell*>* findHiderMoveCells(Cell* startCelll, char segmentsCount);
	bool searchCoveredCells(Cell* cell, Cell* exceptCell, std::list<Cell*>* coveredCells, char count);
	void moveHider(HiderSegmentObject* headSeg);
	void checkHiders();
};

