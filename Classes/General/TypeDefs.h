#pragma once
#include "cocos2d.h"

#include "General/EnumTypes.h"
#include <string>
#include <list>
#include "json/rapidjson.h"
#include "cocostudio/ActionTimeline/CCActionTimelineCache.h"

class Cell;

struct Goal
{
	GoalTypes GoalType = GoalTypes::WaffleObject;
	std::string GameType;
	std::list<std::string>* RoundEndCountTypes;
	int Value;
	void initWithJson(rapidjson::Value& json)
	{
		if (json.IsObject())
		{
			this->GoalType = GoalTypes::_from_string(json["object_type"].GetString());
			this->GameType = json["type"].GetString();
			auto countTypesArray = json["round_end_count_types"].GetArray();
			if (countTypesArray.Size() > 0)
			{
				this->RoundEndCountTypes = new std::list<std::string>();
				for (auto& v : countTypesArray)
				{
					this->RoundEndCountTypes->push_back(v.GetString());
				}
			}
			this->Value = json["value"].GetInt();
		}
	}
};

struct SpawnTable
{
	char Layers;
	Direction direction = Direction::any;
	TileColors Color = TileColors::any;
	float Percent;
	char IntMin;
	char IntMax;
	std::string* Type;
	void initWithJson(rapidjson::Value& json)
	{
		if (json.IsObject())
		{
			auto itr = json.FindMember("layers");
			if (itr != json.MemberEnd() && itr->value.IsInt())
			{
				this->Layers = itr->value.GetInt();
			}

			itr = json.FindMember("direction");
			if (itr != json.MemberEnd() && itr->value.IsString())
			{
				this->direction = Direction::_from_string_nocase(itr->value.GetString());
			}

			itr = json.FindMember("color");
			if (itr != json.MemberEnd() && itr->value.IsString())
			{
				this->Color = TileColors::_from_string_nocase(itr->value.GetString());
				cocos2d::log("StawnTable Color field: %s", this->Color._to_string());
			}

			itr = json.FindMember("percent");
			if (itr != json.MemberEnd())
			{
				if (itr->value.IsFloat())
				{
					this->Percent = itr->value.GetFloat();
					if (this->Percent > 1)
					{
						this->Percent /= 100;
					}
				}
				else if (itr->value.IsString())
				{
					this->Percent = atof(itr->value.GetString());
				}
				else if(itr->value.IsInt())
				{
					this->Percent = itr->value.GetInt();
					if (this->Percent > 1)
					{
						this->Percent /= 100;
					}
				}
			}
			
			
			itr = json.FindMember("int_max");
			if (itr != json.MemberEnd() && itr->value.IsInt())
			{
				this->IntMax = json["int_max"].GetInt();
			}
			
			itr = json.FindMember("int_min");
			if (itr != json.MemberEnd() && itr->value.IsInt())
			{
				this->IntMin = json["int_min"].GetInt();
			}

			itr = json.FindMember("type");
			if (itr != json.MemberEnd() && itr->value.IsString())
			{
				this->Type = new std::string(itr->value.GetString());
				cocos2d::log("StawnTable Type field: %s", this->Type->c_str());
			}
		}
	}
};

typedef std::map<TileColors, float>* TileColorsTable;
typedef std::list<SpawnTable>* SpawnTablesList;
typedef std::list<Cell*> CellsList;

struct GridPos
{
	char Col;
	char Row;
};

struct LiquidSystem
{
	int TurnTimer;
	char LevelMax;
	char FillerToggle;
	char DrainerToggle;
	std::list<GridPos> IgnoreGridPos;
	float DrainStep;
	float FillStep;
	char LevelMin;
	float LevelStep;
	std::list<char> IgnoreColumns;
	std::string Type;
	std::list<char> IgnoreRows;
	float LevelStart;

	bool containsIgnoreColumn(char col) { return std::find(IgnoreColumns.begin(), IgnoreColumns.end(), col) != IgnoreColumns.end(); }
	bool containsIgnoreRow(char row) { return std::find(IgnoreRows.begin(), IgnoreRows.end(), row) != IgnoreRows.end(); }
	bool containsIgnorePos(GridPos& pos)
	{
		return std::any_of(IgnoreGridPos.begin(), IgnoreGridPos.end(), [pos](GridPos ignorePos) { return ignorePos.Col == pos.Col && ignorePos.Row == pos.Row; });
	}
	bool containsIgnorePos(char col, char row)
	{
		return std::any_of(IgnoreGridPos.begin(), IgnoreGridPos.end(), [col, row](GridPos ignorePos) { return ignorePos.Col == col && ignorePos.Row == row; });
	}
};

struct SpawnOnCollectSystem
{
	char IntervalMax;
	char EnsureOne;
	std::string ObjectType;
	std::string Type;
	char IntervalMin;
};

struct ConveyorInfo
{
	char ToColumn;
	char ToRow;
	char FromColumn;
	char FromRow;
};

struct Limit
{
	std::string Type;
	char Value;
};

struct GridSizeT
{
	char Width;
	char Height;
};

template <class T> void* constructor() { return (void*)T::create(); }

struct factory
{
	typedef void*(*constructor_t)();
	typedef std::map<std::string, constructor_t> map_type;
	map_type m_classes;

	template <class T>
	void register_class(std::string const& n)
	{
		m_classes.insert(std::make_pair(n, &constructor<T>));
	}

	void* construct(std::string const& n)
	{
		map_type::iterator i = m_classes.find(n);
		if (i == m_classes.end()) return 0; // or throw or whatever you want
		return i->second();
	}
};

