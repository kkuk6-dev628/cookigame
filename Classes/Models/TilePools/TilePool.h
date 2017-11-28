#pragma once
#include "base/CCVector.h"
#include "Models/Tiles/MovingTile.h"

class TilePool
{
public:
	TilePool();
	~TilePool();

public:
	MovingTile* getTile();
	void putTile(MovingTile*);

	int size() const
	{
		return tiles.size();
	}

protected:
	cocos2d::Vector<MovingTile*> tiles;
};

