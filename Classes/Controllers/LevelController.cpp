#include "LevelController.h"
#include "platform/CCFileUtils.h"
#include "spine/AttachmentVertices.h"
#include "json/document.h"
#include "General/Constants.h"
#include "Models/DataModels/UserData.h"

USING_NS_CC;
using namespace std;

LevelController* LevelController::instance = nullptr;

LevelController::LevelController()
{
	levelCount = 1260;
	//loadCurrentLevel();
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

void LevelController::loadEpisodeInfo(const char episodeNumber)
{
	const auto fileUtils = FileUtils::getInstance();
	if (this->episodeNumber != episodeNumber)
	{
		this->episodeNumber = episodeNumber;
		auto totalLevelsCount = 0;
		const auto episodeFileName = StringUtils::format("levels/%s%d.json", EpisodeFilePrefix.c_str(), episodeNumber);
		auto fileContent = fileUtils->getStringFromFile(episodeFileName);
		this->JsonDoc.Parse(fileContent.c_str());
	}
	//while (fileUtils->isFileExist(episodeFileName))
	//{
	//	this->episodesCount = epNumber;
	//	auto fileContent = fileUtils->getStringFromFile(episodeFileName);
	//	rapidjson::Document epJson;
	//	epJson.Parse<0>(fileContent.c_str());
	//	const auto levelCount = epJson["data"]["level"].Size();
	//	totalLevelsCount += levelCount;
	//	this->episodesInfo.push_back(levelCount);
	//	epNumber++;
	//	episodeFileName = StringUtils::format("levels/%s%d.json", EpisodeFilePrefix.c_str(), epNumber);
	//}
	//cocos2d::log("Read Level Info: episodes count - %d, level count - %d", this->episodesInfo.size(), totalLevelsCount);
}

void LevelController::loadCurrentLevel()
{
	if (this->currentLevel != nullptr)
	{
		delete this->currentLevel;
	}
	this->currentLevel = new Level();
	this->currentLevel->load(UserData::getInstance()->getTopLevel());
}

bool LevelController::setCurrentLevel(int level)
{
	currentLevelNumber = level;
	if (this->currentLevel != nullptr)
	{
		delete this->currentLevel;
	}
	this->currentLevel = new Level();
	this->currentLevel->load(currentLevelNumber);
	return true;
}