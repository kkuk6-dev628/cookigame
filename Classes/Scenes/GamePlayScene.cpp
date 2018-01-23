#include "GamePlayScene.h"
#include "Controllers/GameController.h"
#include "Controllers/BoardController.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"
#include "Layers/Dialogs.h"
#include "LevelMapScene.h"

//ZOrder
enum
{
	kZBack,
	kZBoard,
	kZUI,
	kZEffect,
	kZPopup,
};

GamePlayScene::GamePlayScene()
{
}


GamePlayScene::~GamePlayScene()
{
}

Scene* GamePlayScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GamePlayScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GamePlayScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	rootNode = cocos2d::CSLoader::createNode("res/gamescene.csb");
	rootNode->setContentSize(visibleSize);
	addChild(rootNode);


	boardController = GameController::getInstance()->getBoardController();
	boardController->initWithNode(rootNode);

	auto topMenuArea = rootNode->getChildByName("top_menu_area");
	auto bottomMenuArea = rootNode->getChildByName("bottom_menu_area");

	ui::Button *m_btn_settings = static_cast<ui::Button*>(bottomMenuArea->getChildByName("setting_button"));
	m_btn_settings->addClickEventListener([this](Ref*) {
		this->showSettingsDlg();
	});
	ui::Button *m_btn_spoon = static_cast<ui::Button*>(bottomMenuArea->getChildByName("spoon_button"));
	m_btn_spoon->addClickEventListener([this](Ref*) {
		this->boardController->manualShuffle();
	});

	rootNode->addChild(boardController, kZBoard);
	topMenuArea->setLocalZOrder(kZUI);
	bottomMenuArea->setLocalZOrder(kZUI);

	auto levelTextNode = static_cast<ui::Text*>(topMenuArea->getChildByName("level"));
	levelTextNode->setString(StringUtils::format("Lev %d", LevelController::getInstance()->getCurrentLevel()->getLevelNumber()));
	return true;
}
void GamePlayScene::restartCallback(Ref* pSender)
{
	boardController->removeFromParentAndCleanup(true);
	boardController = GameController::getInstance()->getBoardController(false);
	rootNode->addChild(boardController, kZBoard);
}

void GamePlayScene::restartGame()
{
	boardController->removeFromParentAndCleanup(true);
	boardController = GameController::getInstance()->getBoardController(false);
	rootNode->addChild(boardController, kZBoard);
}

void GamePlayScene::endGame()
{
	LevelMapScene::getInstance()->refresh(true);
	Director::getInstance()->popScene();
}

void GamePlayScene::showSettingsDlg()
{
	if (settingsDlg == nullptr)
	{
		auto dlg = SettingDialog::create();

		dlg->btn_exit->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			this->endGame();
		});
		dlg->btn_continue->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			//resumeGame();
		});
		dlg->btn_close->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			//resumeGame();
		});
		dlg->btn_retry->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			this->restartGame();
		});
		dlg->setOnCloseHandler([this, dlg]() {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			//resumeGame();
		});
		settingsDlg = dlg;
		settingsDlg->retain();
	}
	showPopup(settingsDlg);
}

void GamePlayScene::showPopup(Popup* popup)
{
	if (popup != nullptr)
	{
		//SoundController::playEffectSound(SoundController::SoundEffect::sound_game_window);
		popup->show(this, kZPopup);
	}
}
