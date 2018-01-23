#pragma once
#include <list>
#include "General/TypeDefs.h"
#include "Cell.h"

class FallPath
{
public:
	FallPath();
	~FallPath();
	void pushCell(Cell* cell)
	{
		fallPath.push_front(cell);
		if(cell->containsPortalOut())
		{
			containsPortal = true;
		}
	}
	/*void pushGridPos(char col, char row)
	{
		GridPos gridPos;
		gridPos.Col = col;
		gridPos.Row = row;
		fallPath.push_back(gridPos);
	}*/

	void showFallAction() { startCell->getMovingTile()->showFallAction(this); }

	Cell* startCell = nullptr;
	Cell* endCell = nullptr;
	std::list<Cell*> fallPath;
	bool containsPortal = false;
	bool inWater = false;
private:

};

