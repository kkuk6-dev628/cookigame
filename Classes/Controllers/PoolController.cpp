#include "PoolController.h"
#include "Models/Tiles/LayeredMatchObject.h"
#include "Models/Tiles/LargeTile.h"

PoolController* PoolController::instance = nullptr;
static factory TileClassFactory;
#define REGISTER_CLASS(n) TileClassFactory.register_class<n>(#n)

PoolController::PoolController()
{
	tilesPool = new TilePool;
	RegisterTileClasses();
}


PoolController* PoolController::getInstance()
{
	if (instance == nullptr)
		instance = new PoolController();
	return instance;
}

PoolController::~PoolController()
{
	delete tilesPool;
}

void PoolController::RegisterTileClasses()
{
	REGISTER_CLASS(LayeredMatchObject);
	REGISTER_CLASS(WaffleObject);
	REGISTER_CLASS(ColorPie3Object);

}

CookieTile* PoolController::getCookieTile(std::string typeName)
{
	if (MovingTileTypes::_is_valid_nocase(typeName.c_str()))
	{
		if (tilesPool->size() > 0)
		{
			return tilesPool->getTile();
		}
		else
		{
			return new MovingTile();
		}
	}
	else
	{
		const auto tile = static_cast<CookieTile*>(TileClassFactory.construct(typeName));
		return tile;
	}
}

void PoolController::recycleCookieTile(CookieTile* cookieTile) const
{
	if (MovingTileTypes::_is_valid_nocase(cookieTile->Type.c_str()))
	{
		tilesPool->putTile(static_cast<MovingTile*>(cookieTile));
	}
	else
	{
		CC_SAFE_RELEASE(cookieTile);
	}

}
