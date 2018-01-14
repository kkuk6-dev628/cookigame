#include "LayeredMatchObject.h"
#include "Controllers/SpawnController.h"


LayeredMatchObject::LayeredMatchObject()
{
}


LayeredMatchObject::~LayeredMatchObject()
{
}

void LayeredMatchObject::initWithJson(rapidjson::Value& json)
{
	CookieTile::initWithJson(json);
	if (+TileColors::any == color || color == +TileColors::random)
	{
		color = SpawnController::getInstance()->getSpawnColor();
	}
	const auto textureName = StringUtils::format("%s_%s.png", type.c_str(), color._to_string());
//	initTexture(textureName);
}
