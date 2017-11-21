#include "Level.h"
#include "General/Constants.h"

Level::Level()
{
}


Level::~Level()
{
}

/**
 * \brief 
 * \param levelNumber 
 */
void Level::load(const int levelNumber)
{
	this->levelNumber = levelNumber;
	this->episode = GetEpisodeNumber(levelNumber);

	auto jsonContent = cocos2d::FileUtils::getInstance()->getStringFromFile(GetEpisodeFileName(this->episode));
	rapidjson::Document jsonDoc;
	jsonDoc.Parse<0>(jsonContent.c_str());
	assert(jsonDoc.IsObject());
	auto& levelsJson = jsonDoc["data"]["level"].GetArray();

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

	this->boards = new std::list<BoardData>();

#ifndef NDEBUG
	cocos2d::log("Level number: %d", this->levelNumber);
#endif

	auto& boardsArray = json["boards"].GetArray();
	for (auto& v : boardsArray)
	{
		BoardData board;
		board.initWithJson(v);
		this->boards->push_back(board);
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