#pragma once
#include "platform/CCPlatformMacros.h"
#include "base/CCRef.h"
#include <stdexcept>
#include <list>
#include "Cell.h"
#include "Controllers/PoolController.h"
USING_NS_CC;

class Match : public Ref
{
public:
	~Match();
	static Match* create();
	int matchId = 0;
	bool isWaiting = false;
	Cell* refCell = nullptr;
	std::list<Cell*>* hMatchedCells;
	std::list<Cell*>* vMatchedCells;
	std::list<Cell*>* sMatchedCells;

	int getHMatchCount() const { return hMatchedCells != nullptr ? hMatchedCells->size() : 0; }
	int getVMatchCount() const { return vMatchedCells != nullptr ? vMatchedCells->size() : 0; }
	int getSMatchCount() const { return sMatchedCells != nullptr ? sMatchedCells->size() : 0; }

	void crushAllCells() const;
	void crushMatchedCells() const;
	void crushCell(Cell* cell) const;

private:
	PoolController* poolController;
	Match();
};

