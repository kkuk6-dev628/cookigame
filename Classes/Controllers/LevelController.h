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
	rapidjson::Document JsonDoc;
	char episodeNumber;


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

