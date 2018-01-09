#include "GamePlayScene.h"
#include "2d/CCSprite.h"
#include "Controllers/GameController.h"
#include "Controllers/BoardController.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIButton.h"


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

	auto rootNode = cocos2d::CSLoader::createNode("res/gamescene.csb");
	rootNode->setContentSize(visibleSize);
	addChild(rootNode);

	std::string bgImageName = StringUtils::format("images/Game_%03d_BG.png", GameController::getInstance()->getEpisodeNumber());
	Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto bg = Sprite::create(bgImageName);
	float scale = winSize.height / bg->getContentSize().height;
	bg->setScale(scale, scale);
	bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
	rootNode->addChild(bg);

	rootNode->getChildByName("top_menu_area")->setLocalZOrder(1);
	auto bottomMenuArea = rootNode->getChildByName("bottom_menu_area");
	bottomMenuArea->setLocalZOrder(2);

	ui::Button *m_btn_settings = static_cast<ui::Button*>(bottomMenuArea->getChildByName("setting_button"));
	m_btn_settings->addClickEventListener(CC_CALLBACK_1(GamePlayScene::restartCallback, this));

	boardController = GameController::getInstance()->getBoardController();
	this->addChild(boardController);
	return true;
}
void GamePlayScene::restartCallback(Ref* pSender)
{
	boardController = GameController::getInstance()->getBoardController(false);
	this->addChild(boardController);
}
