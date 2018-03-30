#pragma once
#include "FixTiles.h"

class ColorPieObject : public FixTiles
{
public:
	~ColorPieObject();
	ColorPieObject() :FixTiles() { receiveNearbyAffect = true; noClear = false; }
	CREATE_FUNC(ColorPieObject)
	void showCrushEffect() override;
	bool crush(bool showEffect) override;
	void initTexture() override;

protected:
	Node* getSlotNodeWithColor(std::string nodeName, TileColors slotColor);
	bool setVisibleColorNode(TileColors color);

	Node* rootNode = nullptr;
	std::map<TileColors, Node*>* slots = nullptr;

	char pieNumber = 0;
};

class ColorPie3Object : public ColorPieObject
{
public:
	ColorPie3Object() :ColorPieObject() { pieNumber = 3; }
	CREATE_FUNC(ColorPie3Object)
};

class ColorPie4Object : public ColorPieObject
{
public:
	ColorPie4Object() :ColorPieObject() { pieNumber = 4; }
	CREATE_FUNC(ColorPie4Object)
};

class ColorPie5Object : public ColorPieObject
{
public:
	ColorPie5Object() :ColorPieObject() { pieNumber = 5; }
	CREATE_FUNC(ColorPie5Object)
};

class ColorPie6Object : public ColorPieObject
{
public:
	ColorPie6Object() :ColorPieObject() { pieNumber = 6; }
	CREATE_FUNC(ColorPie6Object)
};