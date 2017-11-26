#pragma once
#include "CookieTile.h"

class FixTiles : public CookieTile
{
public:
	FixTiles();
	~FixTiles();
};

class CrackerObject : public FixTiles
{
public:
	CREATE_FUNC(CrackerObject)
};
