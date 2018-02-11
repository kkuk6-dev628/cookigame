#pragma once
#include "FixTiles.h"

class LayeredCrackerTile : public FixTiles
{
public:
	LayeredCrackerTile() :FixTiles() { receiveNearbyAffect = true; }
	CREATE_FUNC(LayeredCrackerTile);
	void showCrushEffect() override;
	bool crush(bool showEffect) override;
	void initTexture() override;
};

class CrackerObject : public LayeredCrackerTile
{
public:
	CREATE_FUNC(CrackerObject);
};

class ChocolateObject : public LayeredCrackerTile
{
public:
	CREATE_FUNC(ChocolateObject);
};

class WaffleObject : public LayeredCrackerTile
{
public:
	WaffleObject() :LayeredCrackerTile() { receiveNearbyAffect = false; }
	CREATE_FUNC(WaffleObject);

};