#pragma once
#include <list>
#include "Models/DataModels/Level.h"

class LevelController
{
public:
	static LevelController* getInstance();
	~LevelController();

	void loadEpisodeInfo(char episodeNumber);
	void loadCurrentLevel();
	Level* getCurrentLevel() const { return currentLevel; }
	bool setCurrentLevel(int level);
	int getLevelCount() const { return levelCount; }
	rapidjson::Document JsonDoc;
	char episodeNumber;


private:
	LevelController();
	//std::list<int> episodesInfo;
	int episodesCount = 0;
	static LevelController* instance;
	Level* currentLevel = nullptr;
	int currentLevelNumber;
	int levelCount;
};

