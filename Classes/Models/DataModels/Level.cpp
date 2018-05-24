#include "Level.h"
#include "General/Constants.h"
#include "Controllers/LevelController.h"

using namespace rapidjson;

Level::Level()
	:title("level")
	,background("")
{
}


Level::~Level()
{
	this->boards->release();
}

/**
 * \brief 
 * \param levelNumber 
 */
void Level::load(const int levelNumber)
{
	this->levelNumber = levelNumber;
	this->episode = GetEpisodeNumber(levelNumber);

	auto levelCtrl = LevelController::getInstance();
	levelCtrl->loadEpisodeInfo(this->episode);

	assert(levelCtrl->JsonDoc.IsObject());
	auto levelsJson = levelCtrl->JsonDoc["data"]["level"].GetArray();

	for (auto& v : levelsJson)
	{
		if (v["numericId"].GetInt() == levelNumber)
		{
			this->initWithJson(v);
		}
	}
}

void Level::initWithJson(rapidjson::Value& json)
{
	assert(json["numericId"].GetInt() == this->levelNumber);
	assert(json["boards"].IsArray());

	this->boards = cocos2d::__Array::create();
	this->boards->retain();

	//auto& boardsArray = json["boards"].GetArray();
	this->BoardsJson = &(json["boards"]);

	auto boardsArray = json["boards"].GetArray();
	auto& board = boardsArray[0];
	auto& goal = board["goals"].GetArray()[0];
	auto goalType = goal["object_type"].GetString();
	levelType = GoalTypes::_from_string(goalType);

	limit = new std::list<Limit>;
	if(json["limit"].IsArray())
	{
		auto limitArray = json["limit"].GetArray();
		for(auto& item : limitArray)
		{
			Limit lim;
			lim.Type = item["type"].GetString();
			lim.Value = item["value"].GetInt();
			limit->push_back(lim);
		}
	}

	if (json["scores"].IsArray())
	{
		if(scores == nullptr)
		{
			scores = new std::vector<int>;
		}
		auto scoresArray = json["scores"].GetArray();
		for (auto& item : scoresArray)
		{
			scores->push_back(item.GetInt());
		}
	}
}



char Level::GetEpisodeNumber(const int levelNumber)
{
	return levelNumber > 20 ? (levelNumber - 1) / 20 + 2 : (levelNumber > 10 ? 2 : 1);

}

std::string Level::GetEpisodeFileName(const char episodeNumber)
{
	return cocos2d::StringUtils::format("levels/%s%d.json", EpisodeFilePrefix.c_str(), episodeNumber);
}