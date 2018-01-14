#pragma once
#include <list>
#include "General/TypeDefs.h"
#include "Cell.h"

class FallPath
{
public:
	FallPath();
	~FallPath();
	void pushGridPos(GridPos gridPos) { fallPath.push_front(gridPos); }
	void pushGridPos(char col, char row)
	{
		GridPos gridPos;
		gridPos.Col = col;
		gridPos.Row = row;
		fallPath.push_back(gridPos);
	}

	void showFallAction() { startCell->getMovingTile()->showFallAction(this); }

	Cell* startCell = nullptr;
	Cell* endCell = nullptr;
	std::list<GridPos> fallPath;

private:

};

