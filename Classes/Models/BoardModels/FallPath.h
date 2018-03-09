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

	void addPath(FallPath* path) 
	{
		fallPath.push_front(path->endCell);
		for (auto itr = path->fallPath.rbegin(); itr != path->fallPath.rend(); ++itr)
		{
			fallPath.push_front(*itr);
		}
		startCell = path->startCell;
		if(path->containsPortal)
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
	CellsList fallPath;
	bool containsPortal = false;
	bool inWater = false;
private:

};

