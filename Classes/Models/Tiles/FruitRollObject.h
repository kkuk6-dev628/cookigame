#pragma once
#include "FixTiles.h"

class FruitRollGroup;

class FruitRollObject : public FixTiles
{
public:
	FruitRollObject();
	~FruitRollObject();

	CREATE_FUNC(FruitRollObject)

	void initWithJson(rapidjson::Value& json) override;

	void initTexture() override;
	void initTexture(std::string textureName) override;
	bool crush(bool showEffect) override;

	FruitRollGroup* getGroup() const { return group; }
	void setGroup(FruitRollGroup* rollGroup) { group = rollGroup; }
	FruitRollObject* getPrevObject() const { return prevObject; }
	void setPrevObject(FruitRollObject* prev) { prevObject = prev; }
	void hideTexture() const { textureSprite->setVisible(false); }

	void setTextureRotation(Node* node);
	void setOneFruitTexture();
	char getRemainderCount();

	bool isHead = false;

private:
	FruitRollGroup* group = nullptr;
	FruitRollObject* prevObject = nullptr;
};

