#pragma once
#include "Models/Tiles/CookieTile.h"
#include "Models/TilePools/TilePool.h"

class PoolController
{
public:
	static PoolController* getInstance();
	~PoolController();
	void RegisterTileClasses();

	CookieTile* getCookieTile(std::string typeName);
	void recycleCookieTile(CookieTile* cookieTile) const;

private:
	PoolController();
	static PoolController* instance;
	TilePool* tilesPool;
};

