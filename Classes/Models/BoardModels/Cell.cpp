#include "Cell.h"
#include "Models/Tiles/CookieTile.h"

Cell::Cell()
{
	pSourceTile = nullptr;
}


Cell::~Cell()
{
}

void Cell::setSourceTile(CookieTile* pTile)
{
	pSourceTile = pTile;
	pSourceTile->setCell(this);
	isEmpty = false;
}
