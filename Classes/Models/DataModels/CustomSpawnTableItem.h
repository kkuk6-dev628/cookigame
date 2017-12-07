#pragma once
#include <string>
#include "General/TypeDefs.h"

class CustomSpawnTableItem
{
public:
	CustomSpawnTableItem();
	~CustomSpawnTableItem();
	static SpawnTablesList CreateSpawnTablesFromJson(rapidjson::Value& json);
	void initWithJson(rapidjson::Value& json);

private:
	std::string name;
	SpawnTablesList spawnTable;


public:
	std::string getName() const
	{
		return name;
	}

	void setName(const std::string& name)
	{
		this->name = name;
	}

	SpawnTablesList getSpawnTable() const
	{
		return spawnTable;
	}

	void setSpawnTable(SpawnTablesList spawnTables)
	{
		spawnTable = spawnTables;
	}

};

