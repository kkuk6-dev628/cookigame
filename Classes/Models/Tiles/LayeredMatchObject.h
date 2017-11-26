#pragma once
#include "CookieTile.h"
#include "MovingTile.h"

class LayeredMatchObject : public MovingTile
{
public:
	CREATE_FUNC(LayeredMatchObject)
	LayeredMatchObject();
	~LayeredMatchObject();

	void initWithJson(rapidjson::Value& json) override;

};

