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

	static GamePlayScene* getInstance() { return instance; };

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void initEffectNode();

	void onEnterTransitionDidFinish();
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	// implement the "static create()" method manually
	CREATE_FUNC(GamePlayScene)

	void restartCallback(Ref* pSender);
	void restartGame();
	void updateBoosterCount();

private:
	static GamePlayScene* instance;
	BoardController* boardController = nullptr;
	Node* rootNode = nullptr;
	Node* effectNode = nullptr;

	Popup* settingsDlg = nullptr;

	void showSettingsDlg();
	void showStartDlg();
	void showPopup(Popup* popup);
	void endGame();
	void showGameWinDlg();
	void showBoosterBuyDlg(BoosterType boosterType);
};

