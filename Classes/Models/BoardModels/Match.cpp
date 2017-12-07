#include "Match.h"


void Match::crushAllCells() const
{
	crushCell(refCell);
	for (auto& cell : *hMatchedCells) crushCell(cell);
	for (auto& cell : *vMatchedCells) crushCell(cell);
	for (auto& cell : *sMatchedCells) crushCell(cell);
}

void Match::crushMatchedCells() const
{
	crushCell(refCell);
	if (hMatchedCells->size() > 2)
	{
		for (auto& cell : *hMatchedCells) crushCell(cell);
	}
	if (vMatchedCells->size() > 2)
	{
		for (auto& cell : *vMatchedCells) crushCell(cell);
	}
	if (sMatchedCells->size() > 3)
	{
		for (auto& cell : *sMatchedCells) crushCell(cell);
	}
}

void Match::crushCell(Cell* cell) const
{
	if (cell == nullptr || cell->isEmpty || cell->getSourceTile() == nullptr)
	{
		return;
	}
	poolController->recycleCookieTile(cell->getSourceTile());
	cell->clear();
}


Match::Match()
{
	hMatchedCells = new std::list<Cell*>();
	vMatchedCells = new std::list<Cell*>();
	sMatchedCells = new std::list<Cell*>();
	poolController = PoolController::getInstance();
}


Match::~Match()
{
	CC_SAFE_DELETE(hMatchedCells);
	CC_SAFE_DELETE(vMatchedCells);
	CC_SAFE_DELETE(sMatchedCells);
}

Match* Match::create()
{
	auto match = new Match();
	match->autorelease();
	return match;
}
