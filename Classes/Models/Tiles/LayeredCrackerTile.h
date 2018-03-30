#pragma once
#include "FixTiles.h"

class LayeredCrackerTile : public FixTiles
{
public:
	LayeredCrackerTile() :FixTiles() { receiveNearbyAffect = true; noClear = false; }
	CREATE_FUNC(LayeredCrackerTile)
	void showCrushEffect() override;
	bool crush(bool showEffect) override;
	void initTexture() override;
};

class CrackerObject : public LayeredCrackerTile
{
public:
	CrackerObject() : LayeredCrackerTile() {}
	CREATE_FUNC(CrackerObject)
};

class PretzelObject : public LayeredCrackerTile
{
public:
	PretzelObject() : LayeredCrackerTile() {}
	CREATE_FUNC(PretzelObject)
};

class ChocolateObject : public LayeredCrackerTile
{
public:
	CREATE_FUNC(ChocolateObject)
};

class WaffleObject : public LayeredCrackerTile
{
public:
	WaffleObject() :LayeredCrackerTile() { receiveNearbyAffect = false; }
	CREATE_FUNC(WaffleObject)

};

class IceCoverObject : public LayeredCrackerTile
{
public:
	IceCoverObject() : LayeredCrackerTile() { receiveNearbyAffect = false; }
	CREATE_FUNC(IceCoverObject)
};
