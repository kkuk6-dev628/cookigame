#pragma once
#include "CookieTile.h"
#include "Controllers/PoolController.h"

class FixTiles : public CookieTile
{
public:
	FixTiles();
	//~FixTiles();

	void initWithJson(rapidjson::Value& json) override;
	void initTexture() override;
};

class CrackerObject : public FixTiles
{
public:
	CREATE_FUNC(CrackerObject)
};

class ChocolateObject: public FixTiles
{
public:
	ChocolateObject():FixTiles() { receiveNearbyAffect = true; }
	CREATE_FUNC(ChocolateObject);
	void showCrushEffect() override;
};

class PathObject: public FixTiles
{
public:
	CREATE_FUNC(PathObject);

	void initTexture() override;
};

class PathFollowerObject: public FixTiles
{
public:
	CREATE_FUNC(PathFollowerObject);
};

class PathGoalObject: public FixTiles
{
public:
	CREATE_FUNC(PathGoalObject);
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
