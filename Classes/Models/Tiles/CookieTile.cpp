#include "CookieTile.h"
#include "General/Utils.h"
#include "Models/BoardModels/Cell.h"


CookieTile::CookieTile()
	: TileBase()
{
	pCell = nullptr;
}


CookieTile::~CookieTile()
{
}

void CookieTile::setCell(Cell* cell)
{
	pCell = cell;
	gridPos = pCell->gridPos;
}

void CookieTile::initWithJson(rapidjson::Value& json)
{
	this->type = json["type"].GetString();
	auto& data = json["data"];

	auto& itr = data.FindMember("layers");
	if (itr != data.MemberEnd() && itr->value.IsInt())
	{
		this->layers = itr->value.GetInt();
	}

	itr = data.FindMember("color");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		try
		{
			this->color = TileColors::_from_string(itr->value.GetString());
		}
		catch (const std::runtime_error&)
		{
			cocos2d::log("Undefined TileColors Type: %s", itr->value.GetString());
		}
	}

	itr = data.FindMember("direction");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->direction = itr->value.GetString();
	}

	itr = data.FindMember("name");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->name = itr->value.GetString();
	}

	itr = data.FindMember("priority");
	if (itr != data.MemberEnd() && itr->value.IsInt())
	{
		this->priority = itr->value.GetInt();
	}

	itr = data.FindMember("createSpread");
	if (itr != data.MemberEnd() && itr->value.IsBool())
	{
		this->createSpread = itr->value.GetBool();
	}

	itr = data.FindMember("mirror");
	if (itr != data.MemberEnd() && itr->value.IsBool())
	{
		this->mirror = itr->value.GetBool();
	}

	itr = data.FindMember("modifier_type");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		try
		{
			this->modifierType = ModifierTypes::_from_string(itr->value.GetString());
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
		this->gridSize = Utils::StrToGridSize(gridSizeString, "x");
	}

	itr = data.FindMember("setType");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		try
		{
			this->setType = SetTypes::_from_string(itr->value.GetString());
		}
		catch (const std::runtime_error&)
		{
			cocos2d::log("Undefined SetTypes Type: %s", itr->value.GetString());
		}
	}

	itr = data.FindMember("createJelly");
	if (itr != data.MemberEnd() && itr->value.IsBool())
	{
		this->createJelly = itr->value.GetBool();
	}

	itr = data.FindMember("position");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		try
		{
			this->hiderSegPosition = HiderSegmentPosition::_from_string(itr->value.GetString());
		}
		catch (const std::runtime_error&)
		{
			cocos2d::log("Undefined HiderSegmentPosition Type: %s", itr->value.GetString());
		}
	}

	itr = data.FindMember("chain");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->chain = itr->value.GetString();
	}

	itr = data.FindMember("spawn_data");
	if (itr != data.MemberEnd() && itr->value.IsObject())
	{
		this->spawnData.initWithJson(itr->value);
	}

	itr = data.FindMember("spawn_type");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->spawnType = itr->value.GetString();
	}

	itr = data.FindMember("zone");
	if (itr != data.MemberEnd() && itr->value.IsInt())
	{
		this->zone = itr->value.GetInt();
	}

	itr = data.FindMember("spawn_table");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->spawnTableName = itr->value.GetString();
	}

	itr = data.FindMember("slot_1");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->slot1 = TileColors::_from_string(itr->value.GetString());
	}

	itr = data.FindMember("slot_2");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->slot2 = TileColors::_from_string(itr->value.GetString());
	}

	itr = data.FindMember("slot_3");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->slot3 = TileColors::_from_string(itr->value.GetString());
	}

	itr = data.FindMember("slot_4");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->slot4 = TileColors::_from_string(itr->value.GetString());
	}

	itr = data.FindMember("slot_5");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->slot5 = TileColors::_from_string(itr->value.GetString());
	}

	itr = data.FindMember("slot_6");
	if (itr != data.MemberEnd() && itr->value.IsString())
	{
		this->slot6 = TileColors::_from_string(itr->value.GetString());
	}

	itr = data.FindMember("sequence_id");
	if (itr != data.MemberEnd() && itr->value.IsInt())
	{
		this->sequenceId = itr->value.GetInt();
	}

	itr = data.FindMember("power");
	if (itr != data.MemberEnd() && itr->value.IsInt())
	{
		this->power = itr->value.GetInt();
	}

}
