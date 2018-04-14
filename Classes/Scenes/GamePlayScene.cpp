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

GamePlayScene* GamePlayScene::instance = nullptr;

GamePlayScene::GamePlayScene()
{
	instance = this;
}


GamePlayScene::~GamePlayScene()
{
	instance = nullptr;
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
	initEffectNode();
	boardController->initWithNode(rootNode, effectNode);

	auto topMenuArea = rootNode->getChildByName("top_menu_area");
	auto bottomMenuArea = rootNode->getChildByName("bottom_menu_area");

	ui::Button *m_btn_settings = static_cast<ui::Button*>(bottomMenuArea->getChildByName("setting_button"));
	m_btn_settings->addClickEventListener([this](Ref*) {
		this->showSettingsDlg();
		//this->showGameWinDlg();
	});
	ui::Button *m_btn_spoon = static_cast<ui::Button*>(bottomMenuArea->getChildByName("spoon_button"));
	m_btn_spoon->addClickEventListener([this](Ref*) {
		//this->boardController->manualShuffle();
		this->boardController->showGameWinDlg();
	});

	rootNode->addChild(boardController, kZBoard);
	topMenuArea->setLocalZOrder(kZUI);
	bottomMenuArea->setLocalZOrder(kZUI);

	auto levelTextNode = static_cast<ui::Text*>(topMenuArea->getChildByName("level_number"));
	levelTextNode->setString(StringUtils::format("Lev %d", LevelController::getInstance()->getCurrentLevel()->getLevelNumber()));

	return true;
}

void GamePlayScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		showSettingsDlg();
	}
}

void GamePlayScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	SoundController::getInstance()->playBgMusic(SoundController::musicGamePlay);

	// keyboard
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(GamePlayScene::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}

void GamePlayScene::initEffectNode()
{
	effectNode = Node::create();
	rootNode->addChild(effectNode, kZEffect);
	auto const width = CellSize * boardController->getBoardWidth();
	auto const height = CellSize * boardController->getBoardHeight();
	effectNode->setContentSize(Size(width, height));
	const auto originX = CenterX - width / 2;
	const auto originY = CenterY - height / 2;
	effectNode->setPosition(originX, originY);
}

void GamePlayScene::restartCallback(Ref* pSender)
{
	boardController->removeFromParentAndCleanup(true);
	boardController = GameController::getInstance()->getBoardController(false);
	rootNode->addChild(boardController, kZBoard);
}

void GamePlayScene::restartGame()
{
	BoardController::gameState = Idle;
	boardController->removeFromParentAndCleanup(true);
	boardController = GameController::getInstance()->getBoardController(false);
	boardController->initWithNode(rootNode, effectNode);
	rootNode->addChild(boardController, kZBoard);
}

void GamePlayScene::endGame()
{
	BoardController::gameState = Idle;
	LevelMapScene::getInstance()->refresh(true);
	Director::getInstance()->popScene();
}

void GamePlayScene::showSettingsDlg()
{
	if (settingsDlg == nullptr)
	{
		auto dlg = SettingDialog::create();

		auto soundController = SoundController::getInstance();
		auto imgMusicOn = dlg->btn_music->getChildByName("music_on");
		auto imgMusicOff = dlg->btn_music->getChildByName("music_off");
		auto imgSoundOn = dlg->btn_sound->getChildByName("sound_on");
		auto imgSoundOff = dlg->btn_sound->getChildByName("sound_off");

		if (soundController->isBgMusicOn())
		{
			imgMusicOn->setVisible(true);
			imgMusicOff->setVisible(false);
		}
		else
		{
			imgMusicOn->setVisible(false);
			imgMusicOff->setVisible(true);
		}

		if (soundController->isEffectsOn())
		{
			imgSoundOn->setVisible(true);
			imgSoundOff->setVisible(false);
		}
		else
		{
			imgSoundOn->setVisible(false);
			imgSoundOff->setVisible(true);
		}


		dlg->btn_exit->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			this->endGame();
		});
		dlg->btn_continue->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			BoardController::gameState = Idle;
		});
		dlg->btn_close->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			BoardController::gameState = Idle;
		});
		dlg->btn_retry->addClickEventListener([this, dlg](Ref*) {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			dlg->close();
			this->restartGame();
		});
		dlg->btn_music->addClickEventListener([=](Ref*) {
			soundController->toggleBgMusicOn();
			if(soundController->isBgMusicOn())
			{
				imgMusicOn->setVisible(true);
				imgMusicOff->setVisible(false);
			}
			else
			{
				imgMusicOn->setVisible(false);
				imgMusicOff->setVisible(true);
			}
		});
		dlg->btn_sound->addClickEventListener([=](Ref*) {
			soundController->toggleEffectsOn();
			if (soundController->isEffectsOn())
			{
				imgSoundOn->setVisible(true);
				imgSoundOff->setVisible(false);
			}
			else
			{
				imgSoundOn->setVisible(false);
				imgSoundOff->setVisible(true);
			}
		});
		dlg->setOnCloseHandler([this, dlg]() {
			//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
			BoardController::gameState = Idle;
		});
		settingsDlg = dlg;
		settingsDlg->retain();
	}
	BoardController::gameState = Paused;
	showPopup(settingsDlg);
}

void GamePlayScene::showGameWinDlg()
{
	auto dlg = GameWinDialog::create();

	dlg->btn_close->addClickEventListener([this, dlg](Ref*) {
		//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
		dlg->close();
		this->endGame();
	});
	dlg->btn_next->addClickEventListener([this, dlg](Ref*) {
		//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
		dlg->close();
		BoardController::gameState = Idle;
		//GameController::getInstance()->goGamePlay(currentLevel->getLevelNumber() + 1);
	});
	dlg->retain();
	dlg->show(this, LayerId::ShowLayer);

}

void GamePlayScene::showPopup(Popup* popup)
{
	if (popup != nullptr)
	{
		//SoundController::playEffectSound(SoundController::SoundEffect::sound_game_window);
		popup->show(this, kZPopup);
	}
}
