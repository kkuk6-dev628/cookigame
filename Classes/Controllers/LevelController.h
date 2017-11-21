#pragma once
#include "deprecated/CCArray.h"
#include <list>
#include "Models/DataModels/Level.h"

class LevelController
{
public:
	static LevelController* getInstance();
	~LevelController();

	void loadEpisodesInfo();
	void loadCurrentLevel();

private:
	LevelController();
	std::list<int> episodesInfo;
	int episodesCount = 0;
	static LevelController* instance;
	Level* currentLevel;
};

