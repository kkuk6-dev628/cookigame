#include "MainMenuScene.h"
#include "Controllers/GameController.h"


MainMenuScene::MainMenuScene()
{
}


MainMenuScene::~MainMenuScene()
{
}

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	//code here

	std::string episodeImageName = StringUtils::format("Map_%03d_BG.png", GameController::getInstance()->EpisodeNumber);
	Size winSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto bg = Sprite::create(episodeImageName);
	bg->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(bg);
	return true;
}
