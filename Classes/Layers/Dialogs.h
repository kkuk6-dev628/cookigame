#ifndef __DIALOGS_H__
#define __DIALOGS_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Ext/Popup.h"
#include "ui/CocosGUI.h"

class Level;
class BoosterPot;

extern const char* gTargetSpriteNames[];
extern const char* gBoosterFilenames[];

class LevelStartDialog : public Popup
{
public:
	LevelStartDialog();
	static LevelStartDialog* createWithLevel(Level* level, float life);
	bool initWithLevel(Level* level, float life);
	void onShow();
protected:
	void onBeforeShow() { setScale(0); };
	cocos2d::FiniteTimeAction* getDefaultShowAction() { return cocos2d::ScaleTo::create(0.15f, 1.3f); };
	cocos2d::FiniteTimeAction* getDefaultCloseAction() { return cocos2d::ScaleTo::create(0.15f, 0); };
private:
	void onTime(float dt);
	float _life;
};

class BoosterConfirmDialog : public Popup
{
public:
	static BoosterConfirmDialog* createWithBooster(BoosterPot* pot);
	bool initWithBooster(BoosterPot* pot);
protected:
	void onBeforeShow() { setScale(0); };
	cocos2d::FiniteTimeAction* getDefaultShowAction() { return cocos2d::ScaleTo::create(0.15f, 1); };
	cocos2d::FiniteTimeAction* getDefaultCloseAction() { return cocos2d::ScaleTo::create(0.15f, 0); };
};

class OpenLevelDialog : public Popup
{
public:
	OpenLevelDialog();
	static OpenLevelDialog* createWithLevel(Level* level);
	bool initWithLevel(Level* level);
protected:
	void onClickGift(cocos2d::Ref* pSender);
	

protected:
	Level* _level;
	cocos2d::Node* _panel;
	
	cocos2d::Node* rootNode;
	cocos2d::ui::Button* btn_gift;
};

class LevelBoosterDialog : public Popup
{
public:
	LevelBoosterDialog();
	static LevelBoosterDialog* createWithLevel(Level* level, float life);
	bool initWithLevel(Level* level, float life);
	void onShow();
protected:
	void onTime(float dt);
	void onBeforeShow() { setScale(0); };
	cocos2d::FiniteTimeAction* getDefaultShowAction() { return cocos2d::ScaleTo::create(0.15f, 1); };
	cocos2d::FiniteTimeAction* getDefaultCloseAction() { return cocos2d::ScaleTo::create(0.15f, 0); };
protected:
	float _life;
};

class SettingDialog : public Popup
{
public:
	SettingDialog();
	CREATE_FUNC(SettingDialog);

public:
	cocos2d::ui::Button* btn_close;
	cocos2d::ui::Button* btn_music;
	cocos2d::ui::Button* btn_sound;
	cocos2d::ui::Button* btn_exit;
	cocos2d::ui::Button* btn_retry;
	cocos2d::ui::Button* btn_continue;
	cocos2d::ui::ImageView* mute_music;
	cocos2d::ui::ImageView* mute_sound;


};

class GameWinDialog : public Popup
{
public:
	GameWinDialog();
	CREATE_FUNC(GameWinDialog);
	void setStar(int count);

public:
	cocos2d::ui::Button* btn_close;
	cocos2d::ui::Button* btn_next;
	cocos2d::ui::Text* lbl_level;
	cocos2d::ui::Text* lbl_score_title;
	cocos2d::ui::Text* lbl_maxscore;
	cocos2d::ui::Text* lbl_maxscore_title;
	cocos2d::ui::Text* lbl_score;
	cocos2d::Sprite* star1;
	cocos2d::Sprite* star2;
	cocos2d::Sprite* star3;

	Node *m_pUiNode;
	Node *m_rewardedCoin;
};

class GameLoseDialog : public Popup
{
public:
	GameLoseDialog();
	CREATE_FUNC(GameLoseDialog);

public:
	cocos2d::ui::Button* btn_close;
	cocos2d::ui::Button* btn_retry;
	cocos2d::ui::Text* lbl_level;
	cocos2d::ui::Text* lbl_score;
};

class ExitGameDialog : public Popup
{
public:
	ExitGameDialog();
	CREATE_FUNC(ExitGameDialog);

public:
	cocos2d::ui::Button* btn_back;
	cocos2d::ui::Button* btn_exit;
};

class RateDialog :public Popup
{
public:
	RateDialog();
	CREATE_FUNC(RateDialog);
public:
	cocos2d::ui::Button* btn_close;
	cocos2d::ui::Button* btn_rate;

};
class ShopDialog :public Popup
{
private:
	void resetShopState(int adType);
	Node* dlgRoot;
public:
	static ShopDialog* _instance;
	static ShopDialog* getInstance() { return _instance; }
	ShopDialog();	
	CREATE_FUNC(ShopDialog);
	void onClickBtnBuy(int gCount, int adType = 1);
	void rewardedVideoFinish();

};
class BoosterBuyDialog :public Popup {
protected:
	int		nBoosterNum;
	int nFee;
private:
	cocos2d::ui::Text* text_coin;
	cocos2d::ui::ImageView* mImageBooster;
	cocos2d::ui::Text* text_fee;
public:
	BoosterBuyDialog();
	~BoosterBuyDialog();
	CREATE_FUNC(BoosterBuyDialog);
	
	static BoosterBuyDialog* _instance;
	static BoosterBuyDialog* getInstance() { return _instance; }

	void initWithBoosterNumber(int boosterIdx);
	void onClickBuy();
	void updateCoin();
};
class MessageDialog : public Popup
{
public:
	MessageDialog();
	CREATE_FUNC(MessageDialog);
	void setTitle(const char* title);
	void setMessage(const char* message);
	void setAutoClose(bool allowAutoClose,float closeDelay=0);
public:
	cocos2d::ui::Text* text_ok;
	cocos2d::ui::Text* lbl_title;
	cocos2d::ui::Text* lbl_info;
	
};
#endif //__DIALOGS_H__