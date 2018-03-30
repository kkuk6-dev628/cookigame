#include "ConveyorBeltObject.h"



ConveyorBeltObject::ConveyorBeltObject()
{
}


ConveyorBeltObject::~ConveyorBeltObject()
{
}

void ConveyorBeltObject::initTexture()
{
	std::string textureName;
	textureName = StringUtils::format("%s_%s.png", type.c_str(), dirString.c_str());
	TileBase::initTexture(textureName);
}

