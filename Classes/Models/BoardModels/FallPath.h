#pragma once
#include <list>
#include "General/TypeDefs.h"
#include "Cell.h"

class FallPath
{
public:
	FallPath();
	~FallPath();
	void pushCell(Cell* cell);

	void addPath(FallPath* path);

	/*void pushGridPos(char col, char row)
	{
		GridPos gridPos;
		gridPos.Col = col;
		gridPos.Row = row;
		fallPath.push_back(gridPos);
	}*/

	void showFallAction();

	void log();

	Cell* startCell = nullptr;
	Cell* endCell = nullptr;
	CellsList fallPath;
	bool containsPortal = false;
	bool inWater = false;
private:

};

