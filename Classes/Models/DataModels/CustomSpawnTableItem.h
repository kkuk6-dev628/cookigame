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
	std::string name1() const
	{
		return name;
	}

	void name1(const std::string& name)
	{
		this->name = name;
	}

	__declspec(property(get = name1, put = name1)) std::string Name;

	SpawnTablesList spawn_table() const
	{
		return spawnTable;
	}

	void spawn_table(SpawnTablesList spawnTables)
	{
		spawnTable = spawnTables;
	}

	__declspec(property(get = spawn_table, put = spawn_table)) SpawnTablesList SpawnTableData;
};

