#include "BoardLayerData.h"


BoardLayerData::BoardLayerData(char width, char height)
{
	this->layerData = new TileData*[height];
	for (auto i = 0; i < height; i++)
	{
		this->layerData[i] = new TileData[width];
	}
}


BoardLayerData::~BoardLayerData()
{
	for (auto i = 0; i < this->Height; i++)
	{
		delete this->layerData[i];
	}
	delete this->layerData;
}

void BoardLayerData::initWithJson(rapidjson::Value& json)
{
	for (auto itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			
		}
	}

}
