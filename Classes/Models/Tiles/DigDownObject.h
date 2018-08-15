#pragma once
#include "FixTiles.h"
class DigDownObject :
	public FixTiles
{
public:
	DigDownObject();
	~DigDownObject();

	CREATE_FUNC(DigDownObject)

	void initWithJson(rapidjson::Value& json) override;

	void initTexture() override;
	//void initTexture(std::string textureName) override;
	bool crush(bool showEffect) override;
	void showCrushEffect() override;
};

