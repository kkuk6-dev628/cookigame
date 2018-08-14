#pragma once
#include "LevelController.h"
#include "SpawnController.h"
#include "Models/DataModels/UserData.h"
#include "SoundController.h"
#include "BoardController.h"

class GameController
{
public:
	~GameController();
	static GameController* getInstance();
	void goMainMenu();
	void goGamePlay(int levelNumber);
	BoardController* createBoardController(bool next = true);
	BoardController* getBoardController() const { return boardController; }

private:
	GameController();

	void loadGamePlayTextureAtlases();

	static GameController* instance;

	LevelController* levelController = nullptr;
	SpawnController* spawnController = nullptr;
	SoundController* soundController = nullptr;
	UserData* userData = nullptr;

	BoardController* boardController = nullptr;
	rapidjson::Value* boardData = nullptr;
	char episodeNumber = 1;
	Level* currentLevel = nullptr;
	char currentBoardIndex = 0;

public:
	char getEpisodeNumber() const
	{
		return episodeNumber;
	}

	void setEpisodeNumber(const char episodeNumber)
	{
		this->episodeNumber = episodeNumber;
	}
};

