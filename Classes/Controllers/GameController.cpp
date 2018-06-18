#include "GameController.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GamePlayScene.h"
#include "Scenes/LevelMapScene.h"
#include "Models/BoardModels/BoardModel.h"
#include "WaffleBoardController.h"
#include "PathBoardController.h"
#include "HiderBoardController.h"
#include "HopplingBoardController.h"
#include "PopsicleBoardController.h"
#include "TopplingBoardController.h"
#include "THopplingBoardController.h"
#include "DigDownController.h"

USING_NS_CC;

GameController* GameController::instance = nullptr;

GameController::GameController()
{
	this->levelController = LevelController::getInstance();
	this->spawnController = SpawnController::getInstance();
	this->soundController = SoundController::getInstance();
	this->userData = UserData::getInstance();
	this->currentLevel = this->levelController->getCurrentLevel();
}

void GameController::loadGamePlayTextureAtlases()
{
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("atlas/tiles.plist");
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
	if (this->levelController->getCurrentLevel() == nullptr)
	{
		this->episodeNumber = Level::GetEpisodeNumber(this->userData->getTopLevel());
	}
	else
	{
		this->episodeNumber = this->levelController->getCurrentLevel()->getEpisode();
	}
	Director::getInstance()->replaceScene(TransitionFade::create(0.6f, LevelMapScene::createScene()));
}

void GameController::goGamePlay(int levelNumber)
{
	loadGamePlayTextureAtlases();
	levelController->setCurrentLevel(levelNumber);
	this->currentLevel = levelController->getCurrentLevel();
	this->boardData = this->currentLevel->BoardsJson;

	Director::getInstance()->pushScene(TransitionFade::create(0.6f, GamePlayScene::createScene()));
}

BoardController* GameController::getBoardController(bool next)
{
	//if(next) currentBoardIndex++;

	assert(boardData->IsArray() && boardData->Size() > 0);
	if (currentBoardIndex >= boardData->Size())
	{
		currentBoardIndex = -1;
		return nullptr;
	}
	auto boardsArray = boardData->GetArray();
	if (boardController != nullptr)
	{
		boardController->removeFromParent();
		CC_SAFE_RELEASE(boardController);
	}

	auto boardModel = new BoardModel();
	boardModel->initWithJson(boardsArray[currentBoardIndex]);

	auto goalType = boardModel->getGoals()->front().GoalType;
	switch (goalType)
	{
	case GoalTypes::WaffleObject:
		boardController = new WaffleBoardController();
		break;
	case GoalTypes::PathObject:
		boardController = new PathBoardController();
		break;
	case GoalTypes::HiderSegmentObject:
		boardController = new HiderBoardController();
		break;
	case GoalTypes::HopplingObject:
		boardController = new HopplingBoardController();
		break;
	case GoalTypes::PopsicleObject:
		boardController = new PopsicleBoardController();
		break;
	case GoalTypes::TopplingObject:
		boardController = new TopplingBoardController();
		break;
	case GoalTypes::thoppling:
		boardController = new THopplingBoardController();
		break;
	case GoalTypes::DigDownYumbleObject:
		boardController = new DigDownController();
		break;
	default:
		boardController = new BoardController();
		break;
	}
	
	boardController->initWithModel(boardModel);
	return boardController;
}



