#include "GameController.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GamePlayScene.h"

USING_NS_CC;

GameController* GameController::instance = nullptr;

GameController::GameController()
{
	this->levelController = LevelController::getInstance();
	this->spawnController = SpawnController::getInstance();
	this->soundController = SoundController::getInstance();
	this->userData = UserData::getInstance();
}


GameController::~GameController()
{
}

GameController* GameController::getInstance()
{
	if (instance == nullptr)
		instance = new GameController();
	return instance;
}

void GameController::goMainMenu()
{
	if (this->levelController->CurrentLevel == nullptr)
	{
		this->episodeNumber = Level::GetEpisodeNumber(this->userData->TopLevel);
	}
	else
	{
		this->episodeNumber = this->levelController->CurrentLevel->Episode;
	}
	Director::getInstance()->replaceScene(TransitionFade::create(0.6f, MainMenuScene::createScene()));
}

void GameController::goGamePlay()
{
	Director::getInstance()->pushScene(TransitionFade::create(0.6f, GamePlayScene::createScene()));
}
