#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Ext/List.h"
#include "General/Constants.h"
#include "Controllers/LevelController.h"

class LevelButton
{
public:
	LevelButton(cocos2d::Node* csbNode);

	void setListener(const cocos2d::ui::Widget::ccWidgetClickCallback &callback);
	int getLevel();
	void setLevel(int level);
	float getPositionY() { return _btn->getParent()->getPositionY(); }
	float getPositionX() { return _btn->getParent()->getPositionX(); }
	void setScale(float s) {
		_btn->setScale(_btn->getScale()*s);
		_label->setScale(_label->getScale()*s);
		_star->setScale(_star->getScale()*s);

		_initscale = _btn->getScale();
	};
	float getInitScale() { return _initscale; }
protected:
	float _initscale;
	cocos2d::ui::Button* _btn;
	cocos2d::ui::TextAtlas* _label;
	cocos2d::ui::ImageView* _star;
};


class ExNode {
protected:
	cocos2d::Node* _node;
	cocostudio::timeline::ActionTimeline* _action;

public:
	ExNode(cocos2d::Node *node) {
		_node = node;
		_action = dynamic_cast<cocostudio::timeline::ActionTimeline*>(node->getActionByTag(node->getTag()));
	}
	void runTimeLine() {
		if (_action && _node->getNumberOfRunningActions() == 0) {
			_action->gotoFrameAndPlay(0, true);
			_node->runAction(_action);
		}
	}
};


class BgSegment
{
public:
	BgSegment(int id, cocos2d::Node* csbNode);
	~BgSegment();

	int getId() { return _id; }

	float getPosY() { return _bg->getPositionY(); }
	void setPosY(float y) { _bg->setPositionY(y); }

	float getTopY() { return _bg->getPositionY() + _bg->getContentSize().height / 2; }
	float getBottomY() { return _bg->getPositionY() - _bg->getContentSize().height / 2; }

	float getHeight() { return _bg->getContentSize().height; }
	float getButtonPosY(int i) { return _buttons[i]->getPositionY(); }
	float getButtonPosX(int i) { return _buttons[i]->getPositionX(); }

	int getLevelCount() { return _buttons.getCount(); }
	int getFirstLevel() { return _startLevel; }
	int getLastLevel() { return _startLevel == 0 ? 0 : _startLevel + _buttons.getCount() - 1; }

	int getStartLevel() { return _startLevel; }
	void setStartLevel(int startLevel) { _startLevel = startLevel; }

	void refresh(bool reset = false);

protected:
	int _id;
	int _prevStartLevel;
	int _startLevel;
	cocos2d::Node* _bg;
	List<LevelButton*> _buttons;
	List<ExNode*> _objNodes;
};


class LevelMapScene : public cocos2d::Layer, public cocos2d::extension::ScrollViewDelegate
{
public:
	static cocos2d::Scene *createScene();
	static LevelMapScene* getInstance() { return _instance; }

	LevelMapScene();
	~LevelMapScene();

	void onEnterTransitionDidFinish();
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);


	void showSettingDlg();

	void setGoldLabel();
	void refresh(bool reset = false);

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);

	void onSelectLevel(cocos2d::Ref* pSender);

protected:
	void startLevel(int index);

	float getOffsetForLevel(int index, int* pBgSegNo = nullptr);

protected:
	static LevelMapScene* _instance;
	LevelController* _levelManager;
	cocos2d::extension::ScrollView* _scrollView;
	List<BgSegment*> _bgSegments;
	cocos2d::ui::ImageView* _maplock;
	float _prevOffsetY;
	float _hSum;
	int _nSum;
	float _bottomHeight;


	cocos2d::Vec2 getPositionForLevel(int index, int* pBgSegNo = nullptr);
	void gotoLevel(int level);
	cocos2d::Sprite* mHeader;
	void moveHeaderTo(int level, int beforelevel = 0);
	List<Node*> clouds;
	void cloud_thread(float dt);
	float cloudtime;
	void onEnter();
	void onExit();


	cocos2d::ui::Text *mGoldLabel;

	cocos2d::ui::Text* mTextDailyTime;
	void refreshDailyTime();
	void setDailyState();
	void updateDailyTime(float dt);

	cocos2d::ui::Text* mTextVideoAdsTime;
	void setVideoAdsButton();
	void updateVideoAdsTime(float dt);
	void refreshVideoAdsTime();
};


