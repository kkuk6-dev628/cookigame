#pragma once
#include "platform/CCPlatformMacros.h"
#include "base/CCRef.h"
#include <stdexcept>
#include <list>
#include "Cell.h"
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
private:
	Match();
};

