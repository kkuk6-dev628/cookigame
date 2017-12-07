#pragma once
#include "base/CCVector.h"
#include "Models/ShowObjects/TileShowObject.h"

class TileShowObjectPool
{
public:
	TileShowObjectPool();
	~TileShowObjectPool();

	TileShowObject* getTileShow();
	void putTileShow(TileShowObject*);

	int size() const
	{
		return showObjects.size();
	}

protected:
	cocos2d::Vector<TileShowObject*> showObjects;

};

