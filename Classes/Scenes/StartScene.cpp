#include "StartScene.h"
#include "General/Constants.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "ui/UIButton.h"
#include "cocostudio/CocoStudio.h"
#include "spine/SkeletonAnimation.h"

USING_NS_CC;

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//BoardController::setCellSize((visibleSize.width - 9) / 9);

	CenterX = origin.x + visibleSize.width / 2;
	CenterY = visibleSize.height / 2 + origin.y;
    
	/// load cocos studio scene
	auto spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("res/texture/backgraund.plist");
	spriteFrameCache->addSpriteFramesWithFile("res/texture/baner.plist");

	auto rootNode = CSLoader::createNode("res/startScene.csb");
	rootNode->setContentSize(visibleSize);
	addChild(rootNode);

	ui::Button *m_btn_start = static_cast<ui::Button*>(rootNode->getChildByName("play_button"));
	m_btn_start->addClickEventListener(CC_CALLBACK_1(StartScene::menuCloseCallback, this));


	auto action = CSLoader::createTimeline("res/startScene.csb");
	action->play("animation0", true);
	action->setLastFrameCallFunc([=]() {
		action->clearLastFrameCallFunc();
		action->play("animation1", true);
	});
	rootNode->runAction(action);

	//auto skeletonNode = spine::SkeletonAnimation::createWithJsonFile("spineAnimations/HiderSegmentObject.json", "spineAnimations/HiderSegmentObject.atlas");
	//skeletonNode->setAnimation(0, "animation", true);
	//skeletonNode->setName("giftanimation");
	//skeletonNode->setPosition(Vec2(CenterX, CenterY));
	//rootNode->addChild(skeletonNode);

	this->initControllers();
    
	SoundController::getInstance()->playBgMusic(SoundController::musicGameMenu);
    return true;
}


void StartScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
	//auto currLevel = this->levelController->getCurrentLevel();
	//auto board = static_cast<BoardModel*>(currLevel->Boards->getObjectAtIndex(0));
	//auto layer = board->Layers->objectForKey("3");
	this->gameController->goMainMenu();
	//this->gameController->goGamePlay();
}

void StartScene::initControllers()
{
	if (this->gameController != nullptr)
	{
		delete this->gameController;
	}
	this->gameController = GameController::getInstance();
	//this->gameController->loadCurrentLevel();
}

void StartScene::releaseControllers()
{
	if (this->gameController != nullptr)
	{
		delete this->gameController;
	}
}

void StartScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		//GGBridge::exitCP();
#else

#endif
		event->stopPropagation();
	}
}

void StartScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	// keyboard
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(StartScene::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}
