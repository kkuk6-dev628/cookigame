#include "BoardLayerData.h"


BoardLayerData::BoardLayerData(char width, char height)
{
	this->Width = width;
	this->Height = height;
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
		delete [] this->layerData[i];
	}
	delete [] this->layerData;
}

BoardLayerData* BoardLayerData::create(char width, char height)
{
	auto layerData = new BoardLayerData(width, height);
	layerData->autorelease();
	return layerData;
}

void BoardLayerData::initWithJson(rapidjson::Value& json)
{
	for (auto itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			auto gridPos = Utils::StrToGridPos(itr->name.GetString(), "_");
			assert(this->Width > gridPos.Col && this->Height > gridPos.Row);
			this->layerData[gridPos.Row][gridPos.Col].initWithJson(itr->value);
		}
	}

}
