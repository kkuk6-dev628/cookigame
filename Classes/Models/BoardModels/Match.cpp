#include "Match.h"


Match::Match()
{
	hMatchedCells = new std::list<Cell*>();
	vMatchedCells = new std::list<Cell*>();
	sMatchedCells = new std::list<Cell*>();
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
