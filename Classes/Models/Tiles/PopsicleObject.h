#pragma once
#include "LargeTile.h"

class PopsicleObject : public LargeTile
{
public:
	PopsicleObject();
	~PopsicleObject();

	CREATE_FUNC(PopsicleObject)

	void initWithJson(rapidjson::Value& json) override;

	PopsicleType getPopType() const { return popType; }
	Vec2 getLineTarget(float boardWidth, float boardHeight) const;

	void initTexture() override;

private:	
	PopsicleType popType = PopsicleType::normal;
	bool isMirror = false;
};

