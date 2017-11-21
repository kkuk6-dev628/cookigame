#pragma once
#include "cocos2d.h"

#include "General/EnumTypes.h"
#include <string>
#include <list>
#include "json/rapidjson.h"
#include "cocostudio/ActionTimeline/CCActionTimelineCache.h"

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
			auto& countTypesArray = json["round_end_count_types"].GetArray();
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
	TileColors Color = TileColors::any;
	float Percent;
	char IntMin;
	char IntMax;
	std::string Type;
	void initWithJson(rapidjson::Value& json)
	{
		if (json.IsObject())
		{
			auto& itr = json.FindMember("layers");
			if (itr != json.MemberEnd() && itr->value.IsInt())
			{
				this->Layers = itr->value.GetInt();
			}

			itr = json.FindMember("color");
			if (itr != json.MemberEnd() && itr->value.IsString())
			{
				this->Color = TileColors::_from_string(itr->value.GetString());
			}

			itr = json.FindMember("color");
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
				this->Type = itr->value.GetString();
			}
		}
	}
};

typedef std::map<TileColors, float>* TileColorsTable;
typedef std::list<SpawnTable>* SpawnTablesList;

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
	std::list<GridPos>* IgnoreGridPos;
	float DrainStep;
	float FillStep;
	char LevelMin;
	float LevelStep;
	std::list<char> IgnoreColumns;
	std::string Type;
	std::list<char> IgnoreRows;
	float LevelStart;
};

struct SpawnOnCollectSystem
{
	char IntervalMax;
	char EnsureOne;
	std::string ObjectType;
	std::string Type;
	char IntervalMin;
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

