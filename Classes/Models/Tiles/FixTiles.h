#pragma once
#include "CookieTile.h"

class FixTiles : public CookieTile
{
public:
	FixTiles();
	~FixTiles();

	void initWithJson(rapidjson::Value& json) override;
	void initTexture() override;
};

class CrackerObject : public FixTiles
{
public:
	CREATE_FUNC(CrackerObject)
};

class WaffleObject : public FixTiles
{
public:
	CREATE_FUNC(WaffleObject)
};

class SeekerPriorityObject : public FixTiles
{
public:
	void countDownLayer() { layers--; }
};

class InvisibleBrickObject: public FixTiles
{
public:
	CREATE_FUNC(InvisibleBrickObject)

		InvisibleBrickObject();
};

class EmptyObject: public FixTiles
{
public:
	CREATE_FUNC(EmptyObject)
		EmptyObject() { canMatch = false; }
};
