#pragma once
#include "FixTiles.h"

class DisplayCaseObject : public FixTiles
{
public:
	DisplayCaseObject() : FixTiles() { receiveNearbyAffect = false; noClear = false; }
	~DisplayCaseObject();
	CREATE_FUNC(DisplayCaseObject)
	void showCrushEffect() override;
	bool crush(bool showEffect) override;
	void initTexture() override;

	bool refill();

private:
	Node* rootNode = nullptr;
	char totalSpawnedCount = 0;
	void resetNumberNode();
};

