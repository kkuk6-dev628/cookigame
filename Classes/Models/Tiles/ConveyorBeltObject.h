#pragma once
#include "FixTiles.h"
class ConveyorBeltObject :
	public FixTiles
{
public:
	ConveyorBeltObject();
	~ConveyorBeltObject();
	CREATE_FUNC(ConveyorBeltObject)

protected:
	void initTexture() override;
};

