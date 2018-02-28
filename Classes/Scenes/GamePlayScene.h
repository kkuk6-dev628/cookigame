#pragma once
#include "platform/CCPlatformMacros.h"
#include "2d/CCScene.h"
#include "2d/CCLayer.h"
#include "Controllers/BoardController.h"
#include "Ext/Popup.h"

USING_NS_CC; 

class GamePlayScene : public Layer
{
public:
	GamePlayScene();
	~GamePlayScene();

	static Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void initEffectNode();

	// implement the "static create()" method manually
	CREATE_FUNC(GamePlayScene);

	void restartCallback(Ref* pSender);

private:
	BoardController* boardController = nullptr;
	Node* rootNode = nullptr;
	Node* effectNode = nullptr;

	Popup* settingsDlg = nullptr;

	void showSettingsDlg();
	void showPopup(Popup* popup);
	void restartGame();
	void endGame();
	void showGameWinDlg();
};

