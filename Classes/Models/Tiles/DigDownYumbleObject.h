#pragma once
#include "FixTiles.h"

class DigDownYumbleObject 
	:public FixTiles
{
public:
	DigDownYumbleObject();
	~DigDownYumbleObject();

	CREATE_FUNC(DigDownYumbleObject)


	void initTexture() override;

	bool crush(bool showEffect) override;
};

