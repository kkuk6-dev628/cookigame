#pragma once

#include "platform/CCPlatformMacros.h"
#include "2d/CCScene.h"
#include "2d/CCLayer.h"

USING_NS_CC;

class MainMenuScene : public Layer
{
public:
	MainMenuScene();
	~MainMenuScene();
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MainMenuScene);

};

