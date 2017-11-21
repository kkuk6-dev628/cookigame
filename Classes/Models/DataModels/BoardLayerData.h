#pragma once
#include "General/TypeDefs.h"
#include "TileData.h"

class BoardLayerData : public cocos2d::Ref
{
public:
	BoardLayerData(char width, char height);
	~BoardLayerData();

	void initWithJson(rapidjson::Value& json);

	char Width;
	char Height;
	
private:
	TileData** layerData;
};

