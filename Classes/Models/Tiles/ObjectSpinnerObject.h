#pragma once
#include "LargeTile.h"
class ObjectSpinnerObject : public LargeTile
{
public:
	ObjectSpinnerObject();
	~ObjectSpinnerObject();
	CREATE_FUNC(ObjectSpinnerObject)
		
	bool isMovable() override { return false; }
	bool crush(bool showEffect) override { return false; }

	void initTexture() override;
	void initWithJson(rapidjson::Value& json) override;

};

