#include "TilePool.h"
#include "General/Constants.h"


TilePool::TilePool()
	: tiles(MaxTileCount)
{
}


TilePool::~TilePool()
{
}

MovingTile* TilePool::getTile()
{
	if (tiles.size() > 0)
	{
		const auto tile = tiles.back();
		tiles.popBack();
		return tile;
	}
	else
	{
		return nullptr;
	}
}

void TilePool::putTile(MovingTile* tile)
{
	tile->removeFromParent();
	tile->stopAllActions();
	tiles.pushBack(tile);
}
