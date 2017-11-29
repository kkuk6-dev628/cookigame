#include "Cell.h"
#include "Models/Tiles/CookieTile.h"

Cell::Cell()
{
	pSourceTile = nullptr;
	layers = __Dictionary::create();
	layers->retain();
}


Cell::~Cell()
{
	layers->release();
}

void Cell::setSourceTile(CookieTile* pTile)
{
	pSourceTile = pTile;
	pTile->setCell(this);
	layers->setObject(pTile, LayerId::Match);
	isEmpty = false;
}

CookieTile* Cell::getTileAtLayer(LayerId layer) const
{
	const auto tile = static_cast<CookieTile*>(layers->objectForKey(layer._to_integral()));
	return tile;
}

void Cell::setTileToLayer(CookieTile* pTile, const LayerId layer)
{
	if (layer._to_integral() == LayerId::Match)
	{
		isEmpty = false;
	}
	pTile->setCell(this);
	layers->setObject(pTile, layer._to_integral());
}

void Cell::clear()
{
	isEmpty = true;
	pSourceTile = nullptr;
}

Cell* Cell::getFallCell() const
{
	if (fallDirection > 0)
	{
		if (upCell != nullptr)
		{
			return upCell;
		}
	}
	return nullptr;
}
