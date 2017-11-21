#include "LevelController.h"
#include "platform/CCFileUtils.h"
#include "spine/AttachmentVertices.h"
#include "json/document.h"
#include "General/Constants.h"

USING_NS_CC;
using namespace std;

LevelController* LevelController::instance = nullptr;

LevelController::LevelController()
{
}


LevelController* LevelController::getInstance()
{
	if (instance == nullptr)
		instance = new LevelController();
	return instance;
}

LevelController::~LevelController()
{
}

void LevelController::loadEpisodesInfo()
{
	const auto fileUtils = FileUtils::getInstance();

	auto epNumber = 1;
	auto totalLevelsCount = 0;
	auto episodeFileName = StringUtils::format("levels/%s%d.json", EpisodeFilePrefix.c_str(), epNumber);
	while (fileUtils->isFileExist(episodeFileName))
	{
		this->episodesCount = epNumber;
		auto fileContent = fileUtils->getStringFromFile(episodeFileName);
		rapidjson::Document epJson;
		epJson.Parse<0>(fileContent.c_str());
		const auto levelCount = epJson["data"]["level"].Size();
		totalLevelsCount += levelCount;
		this->episodesInfo.push_back(levelCount);
		epNumber++;
		episodeFileName = StringUtils::format("levels/%s%d.json", EpisodeFilePrefix.c_str(), epNumber);
	}
	cocos2d::log("Read Level Info: episodes count - %d, level count - %d", this->episodesInfo.size(), totalLevelsCount);
}

void LevelController::loadCurrentLevel()
{
	//this->currentLevel = new Level();
	//this->currentLevel->load(31);


	for (auto i = 1; i <= 700; i++)
	{
		this->currentLevel = new Level();
		this->currentLevel->load(i);
	}
}
