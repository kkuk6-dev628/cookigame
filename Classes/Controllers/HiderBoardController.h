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
	void crushCell(Cell* pCell, bool forceClear = false) override;

protected:
	void addCellToBoard(char col, char row) override;
	void crushUnderCells(Cell* cell) override;
	bool findPathToTarget(Cell* startCell, Cell* exceptCell, CellsList* paath, Cell* targetCell);
	void processCustomLogic(float dt) override;
	Cell* findSeekerTarget(CellsList* targetsList) const override;
	void checkMoveCount() override;
	CellsList* getSeekerTargets(int count) const override;
	void initHiderGame();

private:
	std::map<std::string, HiderGroup*>* hidersMap = nullptr;
	std::list<HiderSegmentObject*>* pendingHiders = nullptr;
	HiderSegmentObject* findNextSegment(HiderSegmentObject* seg, HiderSegmentObject* pastSeg) const;
	CellsList* findHiderMoveCells(Cell* startCelll, char segmentsCount);
	bool searchCoveredCells(Cell* cell, Cell* exceptCell, CellsList* coveredCells, char count);
	void processPendingHiders();
	void moveHider(HiderSegmentObject* headSeg);
	void checkHiders();
	void showHiderCollectingAction(Vec2& pos);
};

class Coord 
{
public:
	Cell* cell;
	int dist;
	CellsList path;
	Coord(Cell* cell, int dist, CellsList* oldPath) {
		this->cell = cell;
		this->dist = dist;
		if(oldPath != nullptr && oldPath->size() > 0)
		{
			this->path.insert(this->path.begin(), oldPath->begin(), oldPath->end());
		}
		this->path.push_back(this->cell);
	}
};
