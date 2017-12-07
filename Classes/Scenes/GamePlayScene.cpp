#include "GamePlayScene.h"
#include "2d/CCSprite.h"
#include "Controllers/GameController.h"
#include "Controllers/BoardController.h"


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

	//code here

	std::string bgImageName = StringUtils::format("images/Game_%03d_BG.png", GameController::getInstance()->getEpisodeNumber());
	Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto bg = Sprite::create(bgImageName);
	float scale = winSize.height / bg->getContentSize().height;
	bg->setScale(scale, scale);
	bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(bg);
	boardController = GameController::getInstance()->getBoardController();
	this->addChild(boardController);
	return true;
}
