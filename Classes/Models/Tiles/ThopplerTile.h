#pragma once
#include "FixTiles.h"

class ThopplerTile : public FixTiles
{
public:
	ThopplerTile() : FixTiles() { receiveNearbyAffect = true; }
	~ThopplerTile();

	CREATE_FUNC(ThopplerTile);

	bool crush(bool showEffect) override 
	{
		removeFromParentAndCleanup(true);
		return true; 
	}
	void initTexture() override;
	void moveDown()
	{
		rootNode->setPosition(Vec2(CellSize / 2.f + 2, 0));
	}
	virtual void initWithCSB() {};

	Node* rootNode;
};


class TopplingObject : public ThopplerTile
{
public:
	CREATE_FUNC(TopplingObject);

	void initWithCSB() override;
};

class HopplingObject : public ThopplerTile
{
public:
	CREATE_FUNC(HopplingObject);

	void initWithCSB() override;
};
