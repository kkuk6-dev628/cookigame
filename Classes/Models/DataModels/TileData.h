#include "General/EnumTypes.h"
#include "General/Utils.h"

struct TileData
{
	std::string Type;
	char Layers;
	TileColors Color = TileColors::any;
	std::string Direction;
	std::string Name;
	char Priority;
	bool CreateSpread;
	bool Mirror;
	ModifierTypes ModifierType = ModifierTypes::NoShuffleModifier;
	GridSizeT GridSize;
	SetTypes SetType = SetTypes::set;
	bool CreateJelly;
	HiderSegmentPosition HiderSegPosition = HiderSegmentPosition::body;
	std::string Chain;
	std::string SpawnType;
	SpawnTable SpawnData;
	char Zone;
	std::string SpawnTableName;
	TileColors Slot1 = TileColors::any;
	TileColors Slot2 = TileColors::any;
	TileColors Slot3 = TileColors::any;
	TileColors Slot4 = TileColors::any;
	TileColors Slot5 = TileColors::any;
	TileColors Slot6 = TileColors::any;
	char SequenceId;
	char Power;

	void initWithJson(rapidjson::Value& json)
	{
		this->Type = json["type"].GetString();
		auto& data = json["data"];

		auto itr = data.FindMember("layers");
		if (itr != data.MemberEnd() && itr->value.IsInt())
		{
			this->Layers = itr->value.GetInt();
		}

		itr = data.FindMember("color");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			try
			{
				this->Color = TileColors::_from_string(itr->value.GetString());
			}
			catch (const std::runtime_error&)
			{
				cocos2d::log("Undefined TileColors Type: %s", itr->value.GetString());
			}
		}

		itr = data.FindMember("direction");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Direction = itr->value.GetString();
		}

		itr = data.FindMember("name");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Name = itr->value.GetString();
		}

		itr = data.FindMember("priority");
		if (itr != data.MemberEnd() && itr->value.IsInt())
		{
			this->Priority = itr->value.GetInt();
		}

		itr = data.FindMember("createSpread");
		if (itr != data.MemberEnd() && itr->value.IsBool())
		{
			this->CreateSpread = itr->value.GetBool();
		}

		itr = data.FindMember("mirror");
		if (itr != data.MemberEnd() && itr->value.IsBool())
		{
			this->Mirror = itr->value.GetBool();
		}

		itr = data.FindMember("modifier_type");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			try
			{
				this->ModifierType = ModifierTypes::_from_string(itr->value.GetString());
			}
			catch (const std::runtime_error&)
			{
				cocos2d::log("Undefined ModifierTypes Type: %s", itr->value.GetString());
			}
		}

		itr = data.FindMember("gridSize");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			auto gridSizeString = itr->value.GetString();
			this->GridSize = Utils::StrToGridSize(gridSizeString, "x");
		}

		itr = data.FindMember("setType");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			try
			{
				this->SetType = SetTypes::_from_string(itr->value.GetString());
			}
			catch (const std::runtime_error&)
			{
				cocos2d::log("Undefined SetTypes Type: %s", itr->value.GetString());
			}
		}

		itr = data.FindMember("createJelly");
		if (itr != data.MemberEnd() && itr->value.IsBool())
		{
			this->CreateJelly = itr->value.GetBool();
		}

		itr = data.FindMember("position");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			try
			{
				this->HiderSegPosition = HiderSegmentPosition::_from_string(itr->value.GetString());
			}
			catch (const std::runtime_error&)
			{
				cocos2d::log("Undefined HiderSegmentPosition Type: %s", itr->value.GetString());
			}
		}

		itr = data.FindMember("chain");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Chain = itr->value.GetString();
		}

		itr = data.FindMember("spawn_data");
		if (itr != data.MemberEnd() && itr->value.IsObject())
		{
			this->SpawnData.initWithJson(itr->value);
		}

		itr = data.FindMember("spawn_type");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->SpawnType = itr->value.GetString();
		}

		itr = data.FindMember("zone");
		if (itr != data.MemberEnd() && itr->value.IsInt())
		{
			this->Zone = itr->value.GetInt();
		}

		itr = data.FindMember("spawn_table");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->SpawnTableName = itr->value.GetString();
		}

		itr = data.FindMember("slot_1");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Slot1 = TileColors::_from_string(itr->value.GetString());
		}

		itr = data.FindMember("slot_2");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Slot2 = TileColors::_from_string(itr->value.GetString());
		}

		itr = data.FindMember("slot_3");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Slot3 = TileColors::_from_string(itr->value.GetString());
		}

		itr = data.FindMember("slot_4");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Slot4 = TileColors::_from_string(itr->value.GetString());
		}

		itr = data.FindMember("slot_5");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Slot5 = TileColors::_from_string(itr->value.GetString());
		}

		itr = data.FindMember("slot_6");
		if (itr != data.MemberEnd() && itr->value.IsString())
		{
			this->Slot6 = TileColors::_from_string(itr->value.GetString());
		}

		itr = data.FindMember("sequence_id");
		if (itr != data.MemberEnd() && itr->value.IsInt())
		{
			this->SequenceId = itr->value.GetInt();
		}

		itr = data.FindMember("power");
		if (itr != data.MemberEnd() && itr->value.IsInt())
		{
			this->Power = itr->value.GetInt();
		}

	}
};
