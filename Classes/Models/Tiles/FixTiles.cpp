#include "FixTiles.h"


FixTiles::FixTiles()
{
}


FixTiles::~FixTiles()
{
}

void FixTiles::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	initTexture();
}

void FixTiles::initTexture()
{
	canMatch = false;
	std::string textureName;
	if(layers != -1)
	{
		textureName = StringUtils::format("%s_%d.png", type.c_str(), layers);
	}
	else
	{
		textureName = StringUtils::format("%s.png", type.c_str());
	}
	TileBase::initTexture(textureName);
}

InvisibleBrickObject::InvisibleBrickObject()
{
	canMatch = false;
	
}