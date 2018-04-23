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
	bool isMovable() override { return false; }
};


class PortalInletObject : public FixTiles
{
public:
	CREATE_FUNC(PortalInletObject)
	void initTexture() override;
	char passedCount = 0;
	void setInWater();
};

class PortalOutletObject : public FixTiles
{
public:
	CREATE_FUNC(PortalOutletObject)
	void initTexture() override;
	void setInWater();
	char passedCount = 0;
};

class PathObject: public FixTiles
{
public:
	CREATE_FUNC(PathObject)

	void initTexture() override;
};

class PathFollowerObject: public FixTiles
{
public:
	CREATE_FUNC(PathFollowerObject)

	void initTexture() override;
	bool crush(bool showEffect) override { return false; };
	Node* rootNode = nullptr;
};

class PathGoalObject: public FixTiles
{
public:
	CREATE_FUNC(PathGoalObject)
	bool crush(bool showEffect) override { return false; };
};

class PieceSwapperObject : public FixTiles
{
public:
	CREATE_FUNC(PieceSwapperObject)

	void initTexture() override;
};

#pragma region invisible objects

class SeekerPriorityObject : public FixTiles
{
public:
	void countDownLayer() { layers--; }
};

class LavaCakeTargetObject : public FixTiles
{
};

class InvisibleBrickObject : public FixTiles
{
public:
	CREATE_FUNC(InvisibleBrickObject)
	InvisibleBrickObject();
};

class EmptyObject : public FixTiles
{
public:
	CREATE_FUNC(EmptyObject)
	EmptyObject() { canMatch = false; }
};

class PointerObject : public FixTiles
{
public:
	CREATE_FUNC(PointerObject)
	PointerObject() { canMatch = false; }
};

#pragma endregion
