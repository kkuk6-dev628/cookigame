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
	Level* getCurrentLevel(){ return currentLevel; }

private:
	LevelController();
	std::list<int> episodesInfo;
	int episodesCount = 0;
	static LevelController* instance;
	Level* currentLevel = nullptr;


#pragma region Get Set Methods
public:
	Level* current_level() const
	{
		return currentLevel;
	}

	__declspec(property(get = current_level)) Level* CurrentLevel;


#pragma endregion
};

