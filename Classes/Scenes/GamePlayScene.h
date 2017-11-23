#pragma once
#include "platform/CCPlatformMacros.h"
#include "2d/CCScene.h"
#include "2d/CCLayer.h"
#include "Controllers/BoardController.h"

USING_NS_CC; 

class GamePlayScene : public Layer
{
public:
	GamePlayScene();
	~GamePlayScene();

	static Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GamePlayScene);

private:
	BoardController* boardController;
};

