#include "StartScene.h"
#include "General/Constants.h"


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

	CellSize = (visibleSize.width - 9) / 9;
	CenterX = origin.x + visibleSize.width / 2;
	CenterY = visibleSize.height / 2 + origin.y;
    
	/////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "images/GreenButton.png",
                                           "images/GreenButton.png",
                                           CC_CALLBACK_1(StartScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		visibleSize.height / 2 + origin.y));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello Cookie", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "StartScene" splash screen"
    auto sprite = Sprite::create("images/Social_000_BG.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScale(3, 3);

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	this->initControllers();
    
    return true;
}


void StartScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
	//auto currLevel = this->levelController->getCurrentLevel();
	//auto board = static_cast<BoardModel*>(currLevel->Boards->getObjectAtIndex(0));
	//auto layer = board->Layers->objectForKey("3");
	//this->gameController->goMainMenu();
	this->gameController->goGamePlay();
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
