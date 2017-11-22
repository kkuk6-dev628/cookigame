#pragma once
#include "General/TypeDefs.h"
#include "TileData.h"

class BoardLayerData : public cocos2d::Ref
{
public:
	~BoardLayerData();
	static BoardLayerData* create(char width, char height);
	void initWithJson(rapidjson::Value& json);

private:
	BoardLayerData(char width, char height);

public:
	char Width;
	char Height;
	
private:
	TileData** layerData;
};

