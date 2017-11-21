#include "CustomSpawnTableItem.h"


CustomSpawnTableItem::CustomSpawnTableItem()
{
}


CustomSpawnTableItem::~CustomSpawnTableItem()
{
}

SpawnTablesList CustomSpawnTableItem::LoadSpawnTablesFromJson(rapidjson::Value& json)
{
	if (json.IsArray() && json.Size() > 0)
	{
		auto ret = new std::list<SpawnTable>();
		auto& arr = json.GetArray();

		for (auto& conv : arr)
		{
			SpawnTable spt;
			spt.initWithJson(conv);
			ret->push_back(spt);
		}
		return ret;
	}
	return nullptr;
}


void CustomSpawnTableItem::initWithJson(rapidjson::Value& json)
{
	assert(json.IsObject());
	this->name = json["name"].GetString();
	this->spawnTable = LoadSpawnTablesFromJson(json["spawn_table"]);
	if (this->spawnTable != nullptr){
		cocos2d::log("custom spawn tables: name - %s, spawn table count - %d", this->name.c_str(), this->spawnTable->size());
	}
	else
	{
		cocos2d::log("custom spawn tables: name - %s, no spawn table data", this->name.c_str());
	}
}
