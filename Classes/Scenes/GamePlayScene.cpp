#include "GamePlayScene.h"
#include "2d/CCSprite.h"
#include "Controllers/GameController.h"
#include "Controllers/BoardController.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/CocosGUI.h"


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

	//std::string bgImageName = StringUtils::format("images/Game_%03d_BG.png", GameController::getInstance()->getEpisodeNumber());
	std::string bgImageName = StringUtils::format("images/Game_001_BG.png");
	Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto bg = Sprite::create(bgImageName);
	float scale = winSize.height / bg->getContentSize().height;
	bg->setScale(scale, scale);
	bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
	rootNode->addChild(bg);

	auto topMenuNode = rootNode->getChildByName("top_menu_area");
	topMenuNode->setLocalZOrder(1);
	auto bottomMenuArea = rootNode->getChildByName("bottom_menu_area");
	bottomMenuArea->setLocalZOrder(2);

	ui::Button *m_btn_settings = static_cast<ui::Button*>(bottomMenuArea->getChildByName("setting_button"));
	m_btn_settings->addClickEventListener(CC_CALLBACK_1(GamePlayScene::restartCallback, this));

	boardController = GameController::getInstance()->getBoardController();

	auto goal = boardController->getGoalType();
	std::string goalNodeName = "FileNode_";
	switch (goal)
	{
	case GoalTypes::PathObject:
		goalNodeName += "3";
		break;
	case GoalTypes::HiderSegmentObject:
		goalNodeName += "1";
		break;
	case GoalTypes::HopplingObject:
		goalNodeName += "5";
		break;
	case GoalTypes::PopsicleObject:
		goalNodeName += "4";
		break;
	case GoalTypes::TopplingObject:
	case GoalTypes::thoppling:
		goalNodeName += "2";
		break;
	default:
		goalNodeName += "6";
		break;
	}

	auto circleGroupNode = topMenuNode->getChildByName("top_menu_circle");
	for (char i = 1; i <= 6; i++)
	{
		auto nodeName = StringUtils::format("FileNode_%d", i);
		circleGroupNode->getChildByName(nodeName)->setVisible(false);
	}

	circleGroupNode->getChildByName(goalNodeName)->setVisible(true);
	this->addChild(boardController);

	auto levelTextNode = static_cast<ui::Text*>(topMenuNode->getChildByName("level"));
	levelTextNode->setString(StringUtils::format("Lev %d", LevelController::getInstance()->getCurrentLevel()->getLevelNumber()));
	return true;
}
void GamePlayScene::restartCallback(Ref* pSender)
{
	boardController = GameController::getInstance()->getBoardController(false);
	this->addChild(boardController);
}
