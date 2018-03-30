#pragma once
#include "LargeTile.h"

class LavaCakeObject : public LargeTile
{
public:
	LavaCakeObject();
	~LavaCakeObject();
	CREATE_FUNC(LavaCakeObject)

	bool crush(bool showEffect) override;
	bool isMovable() override { return false; }

	void initTexture() override;
	void initWithJson(rapidjson::Value& json) override;
	void setIncreaseLavaCakeFlag(bool increase);

private:
	bool increaseLevel;
	char lavaLevel = 1;
};

