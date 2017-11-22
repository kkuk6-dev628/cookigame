#include "CustomSpawnTableItem.h"


CustomSpawnTableItem::CustomSpawnTableItem()
{
}


CustomSpawnTableItem::~CustomSpawnTableItem()
{
}

SpawnTablesList CustomSpawnTableItem::CreateSpawnTablesFromJson(rapidjson::Value& json)
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
	this->spawnTable = CreateSpawnTablesFromJson(json["spawn_table"]);
}
