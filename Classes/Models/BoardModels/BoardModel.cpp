#include "deprecated/CCDictionary.h"
#include "BoardModel.h"
#include "BoardLayerModel.h"


BoardModel::BoardModel()
{
	this->boardLayers = cocos2d::__Dictionary::create();
	this->boardLayers->retain();

}


BoardModel::~BoardModel()
{
	this->boardLayers->release();
}

//BoardModel* BoardModel::create()
//{
//	BoardModel* boardData = new BoardModel();
//	boardData->autorelease();
//	return boardData;
//}

TileColorsTable BoardModel::CreateColorsTableFromJson(const rapidjson::Value& json)
{
	if (json.IsObject())
	{
		auto ret = new std::map<TileColors, float>();
		for (size_t index = 0; index < TileColors::_size(); ++index) {
			auto color = TileColors::_values()[index];
			if (json.HasMember(color._to_string()))
			{
				ret->insert({ color, json[color._to_string()].GetFloat() });
			}
		}
		return ret;
	}
	return nullptr;
}

std::list<CustomSpawnTableItem>* BoardModel::CreateCustomSpawnTablesListFromJson(rapidjson::Value& json)
{
	if (json.IsArray() && json.Size() > 0)
	{
		auto ret = new std::list<CustomSpawnTableItem>();
		auto& arr = json.GetArray();

		for (auto& conv : arr)
		{
			CustomSpawnTableItem spt;
			spt.initWithJson(conv);
			ret->push_back(spt);
		}
		return ret;
	}
	return nullptr;
}

void BoardModel::initWithJson(rapidjson::Value& json)
{
	assert(json.IsObject());
	this->conveyorSpawnTable = CustomSpawnTableItem::CreateSpawnTablesFromJson(json["conveyor_spawn_table"]);
	this->spawnTable = CustomSpawnTableItem::CreateSpawnTablesFromJson(json["spawn_table"]);
	this->transitionOut = json["transitionOut"].GetString();
	this->width = json["width"].GetInt();
	this->height = json["height"].GetInt();
	this->colors = CreateColorsTableFromJson(json["colors"]);
	this->colorsEasy = CreateColorsTableFromJson(json["colorsEasy"]);
	this->goals = new std::list<Goal>();
	auto& goalsArray = json["goals"].GetArray();
	for (auto& v : goalsArray)
	{
		Goal goal;
		goal.initWithJson(v);
		this->goals->push_back(goal);
	}

	this->customSpawnTable = CreateCustomSpawnTablesListFromJson(json["custom_spawn_table"]);

	auto& layersJson = json["layers"];
	for (auto itr = layersJson.MemberBegin(); itr != layersJson.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			auto boardLayer = BoardLayerModel::create(this->width, this->height);
			boardLayer->initWithJson(itr->value);
			const auto layerIndex = atoi(itr->name.GetString());
			this->boardLayers->setObject(boardLayer, layerIndex);
		}
	}
}
