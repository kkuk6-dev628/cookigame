#include "TileShowObjectPool.h"
#include "General/Constants.h"


TileShowObjectPool::TileShowObjectPool()
	: showObjects(MaxTileCount)
{
}


TileShowObjectPool::~TileShowObjectPool()
{
}

TileShowObject* TileShowObjectPool::getTileShow()
{
	if (showObjects.size() > 0)
	{
		const auto tile = showObjects.back();
		showObjects.popBack();
		return tile;
	}
	else
	{
		return nullptr;
	}
}

void TileShowObjectPool::putTileShow(TileShowObject* tile)
{
	tile->removeFromParent();
	tile->stopAllActions();
	showObjects.pushBack(tile);
}