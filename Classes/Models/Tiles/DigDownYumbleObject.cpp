#include "DigDownYumbleObject.h"
#include "Models/BoardModels/Cell.h"


DigDownYumbleObject::DigDownYumbleObject()
{
	receiveNearbyAffect = true;
	canMatch = false;
}


DigDownYumbleObject::~DigDownYumbleObject()
{
}

void DigDownYumbleObject::initTexture()
{
	FixTiles::initTexture();
}

bool DigDownYumbleObject::crush(bool showEffect)
{
	auto digDownObj = pCell->getTileAtLayer(LayerId::Cover);
	if (digDownObj != nullptr && digDownObj->getType() == DIGDOWNOBJECT)
	{
		if (digDownObj->crush(true))
		{
			removeFromParent();
			pCell->clear();
			return true;
		}
	}
	return false;
}
