#pragma once
#include "Cell.h"

class PieceSwapper
{
public:
	PieceSwapper();
	~PieceSwapper();

	void addCell(Cell* cell) const { swappingCells->push_back(cell); }

	void moveTileToCell(MovingTile* tile, Cell* lastCell);
	void swap();
	CellsList* swappingCells = nullptr;
};

