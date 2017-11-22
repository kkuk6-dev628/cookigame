#pragma once
#include "LevelController.h"
#include "SpawnController.h"
#include "Models/DataModels/UserData.h"
#include "SoundController.h"

class GameController
{
public:
	~GameController();
	static GameController* getInstance();
	void goMainMenu();
	void goGamePlay();

private:
	GameController();

	static GameController* instance;

	LevelController* levelController;
	SpawnController* spawnController;
	SoundController* soundController;
	UserData* userData;

	char episodeNumber = 1;


public:
	char episode_number() const
	{
		return episodeNumber;
	}

	void episode_number(char episodeNumber)
	{
		this->episodeNumber = episodeNumber;
	}

	__declspec(property(get = episode_number, put = episode_number)) char EpisodeNumber;
};

