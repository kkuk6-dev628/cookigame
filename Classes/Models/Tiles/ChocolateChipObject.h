#pragma once
#include "MovingTile.h"

class ChocolateChipObject : public MovingTile
{
public:
	ChocolateChipObject();

	CREATE_FUNC(ChocolateChipObject);

	bool crush(bool showEffect) override;
	void showCrushEffect() override;

	void recycle();
};

class DonutObject : public MovingTile
{
public:
	DonutObject();

	CREATE_FUNC(DonutObject);

	bool crush(bool showEffect) override;
	void showCrushEffect() override;

	void recycle();
};