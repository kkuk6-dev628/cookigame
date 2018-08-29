#include "LevelMapScene.h"
#include "Models/DataModels/UserData.h"
#include "cocostudio/CCComExtensionData.h"
#include "Controllers/SoundController.h"
#include "StartScene.h"
#include "GamePlayScene.h"
#include "Layers/Dialogs.h"
#include "Native/AdsControl.h"
#include "AppDelegate.h"

bool g_bDebugMode = true;

#define TARGET_LEVEL 10

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace cocostudio;

enum {
	kZNormal,
	kZUi,
	kZPopup,
};


#pragma region Class LevelButton

LevelButton::LevelButton(Node* csbNode)
{
	_btn = static_cast<Button*>(csbNode->getChildByName("btn"));
	_label = static_cast<TextAtlas*>(csbNode->getChildByName("label"));
	_star = static_cast<ImageView*>(csbNode->getChildByName("star"));
}

void LevelButton::setListener(const ui::Widget::ccWidgetClickCallback &callback)
{
	_btn->addClickEventListener(callback);
}

int LevelButton::getLevel()
{
	return _btn->getTag();
}

void LevelButton::setLevel(int level)
{
	if (level > 0 && level <= LevelController::getInstance()->getLevelCount())
	{
		_btn->getParent()->setVisible(true);
		_btn->stopAllActions();
		if (level <= UserData::getInstance()->getTopLevel())
		{

			_btn->setEnabled(true);
			_btn->setTag(level);
			_label->setVisible(true);
			_label->setString(StringUtils::toString(level));
			int star = UserData::getInstance()->getLevelStar(level);
			if (star > 0) {
				_star->loadTexture(__String::createWithFormat("images/star%d.png", star)->getCString(), cocos2d::ui::TextureResType::LOCAL);
				_star->setVisible(true);
			}
			else {
				_star->setVisible(false);
			}

		}
		else
		{
			_btn->setEnabled(false);
			_label->setVisible(false);
			_star->setVisible(false);
		}
		if (level == UserData::getInstance()->getTopLevel()) {
			float bscale = this->getInitScale();
			_btn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1, bscale*1.2), ScaleTo::create(1, bscale), NULL)));
		}
	}
	else
		_btn->getParent()->setVisible(false);

}

#pragma endregion


#pragma region Class BgSegment

BgSegment::~BgSegment()
{
	for (int i = 0; i < _buttons.getCount(); i++)
		delete _buttons[i];
	for (int i = 0; i < _objNodes.getCount(); i++)
		delete _objNodes[i];
}

BgSegment::BgSegment(int id, Node* csbNode)
{
	_id = id;
	_prevStartLevel = 0;
	_startLevel = 0;

	_bg = csbNode;

	auto levelRoot = csbNode->getChildByName("level");
	auto aniRoot = csbNode->getChildByName("ani");
	if (levelRoot) {
		for (int i = 0; i < levelRoot->getChildrenCount(); i++)
		{
			auto node = levelRoot->getChildren().at(i);

			auto btn = new LevelButton(node);
			btn->setScale(_bg->getScale());
			btn->setListener(CC_CALLBACK_1(LevelMapScene::onSelectLevel, LevelMapScene::getInstance()));
			_buttons.add(btn);



		}
	}
	if (aniRoot) {
		for (int i = 0; i < aniRoot->getChildrenCount(); i++) {
			auto node = aniRoot->getChildren().at(i);
			ComExtensionData* data = dynamic_cast<ComExtensionData*>(node->getComponent("ComExtensionData"));
			auto userdata = data->getCustomProperty();
			_objNodes.add(new ExNode(node));

		}
	}
}

void BgSegment::refresh(bool reset)
{
	if (_startLevel > 0 && (_startLevel != _prevStartLevel || reset))
	{
		for (int i = 0; i < _buttons.getCount(); i++)
			_buttons[i]->setLevel(_startLevel + i);
		_prevStartLevel = _startLevel;
	}
	for (int i = 0; i < _objNodes.getCount(); i++) {
		auto node = _objNodes[i];
		node->runTimeLine();
	}
}

#pragma endregion


#pragma region Class LevelMapScene

LevelMapScene* LevelMapScene::_instance = nullptr;

Scene * LevelMapScene::createScene()
{
	auto scene = Scene::create();
	auto layer = new LevelMapScene();
	layer->autorelease();
	scene->addChild(layer);
	return scene;
}

LevelMapScene::~LevelMapScene()
{
	_instance = nullptr;
	for (int i = 0; i < _bgSegments.getCount(); i++)
		delete _bgSegments[i];
}

#define BG_SEG_COUNT	4

LevelMapScene::LevelMapScene()
{
	_instance = this;
	_levelManager = LevelController::getInstance();
	_prevOffsetY = 0;

	//create scrollview
	_scrollView = cocos2d::extension::ScrollView::create();
	_scrollView->setDelegate(this);
	_scrollView->setViewSize(DesignResolutionSize);
	_scrollView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	_scrollView->setClippingToBounds(true);
	addChild(_scrollView);

	//make segments
	auto nd = CSLoader::createNode("res/LevelSelectScene.csb");
	auto bg0 = nd->getChildByName("bg_map_ep0");
	bg0->removeFromParent();
	bg0->setPosition(DesignResolutionSize.width / 2, bg0->getContentSize().height / 2);
	_scrollView->addChild(bg0);

	_bottomHeight = bg0->getContentSize().height;
	_hSum = _bottomHeight;
	_nSum = 0;
	int i = 0;
	for (i = 1; i <= BG_SEG_COUNT; i++)
	{
		auto seg = nd->getChildByName(String::createWithFormat("bg_map_ep%d", i)->getCString());

		auto bgSeg = new BgSegment(i - 1, seg);
		bgSeg->setStartLevel(_nSum + 1);
		_bgSegments.add(bgSeg);

		seg->removeFromParent();
		seg->setPosition(DesignResolutionSize.width / 2, _hSum + seg->getContentSize().height / 2);
		_scrollView->addChild(seg);

		_hSum += seg->getContentSize().height;
		_nSum += bgSeg->getLevelCount();
	}
	_hSum -= _bottomHeight;
	_prevOffsetY = -_hSum / 3;

	_maplock = (ImageView*)nd->getChildByName("maplock");
	_maplock->removeFromParent();
	_scrollView->addChild(_maplock);
	_maplock->setLocalZOrder(2001);


	mHeader = Sprite::create("res/texture/Map/level_indicator.png");
	mHeader->setAnchorPoint(Point(0.5f, 0));
	mHeader->setScale(0.5f);
	_scrollView->addChild(mHeader);

	auto rootNode = CSLoader::createNode("res/LevelUi.csb");
	addChild(rootNode, kZUi);

	auto btnSettigns = static_cast<ui::Button*>(rootNode->getChildByName("btn_settings"));
	btnSettigns->addClickEventListener([this](Ref*) {
		//SoundController::playEffectSound(SoundController::SoundEffect::sound_game_buttonclick);
		showSettingDlg();
	});
	btnSettigns->setPositionY(DesignResolutionSize.height * 0.85);

	auto btnCoin = static_cast<ui::Button*>(rootNode->getChildByName("btn_coin"));
	btnCoin->addClickEventListener([this](Ref*) {
		ShopDialog::create()->show(this, kZPopup);
	});
	btnCoin->setPositionY(DesignResolutionSize.height * 0.85);

	mGoldLabel = static_cast<ui::Text*>(btnCoin->getChildByName("lbl_coin"));

	auto nGoldCount = UserData::getInstance()->getGold();
	mGoldLabel->setString(StringUtils::toString(nGoldCount));

	auto btn_daily = static_cast<ui::Button*>(rootNode->getChildByName("btn_daily"));
	btn_daily->setVisible(false);
	//btn_daily->addClickEventListener([this](Ref*) {
	//	//DailyLayer::create()->show(this, 5);
	//});
	//mTextDailyTime = (Text*)btn_daily->getChildByName("text_time");

	auto btn_videoAds = static_cast<ui::Button*>(rootNode->getChildByName("btn_gift"));
	//btn_videoAds->addClickEventListener([this](Ref*) {
	//	//VideoBoosterFreeLayer::create()->show(this, 5);
	//});
	btn_videoAds->setVisible(false);
	//mTextVideoAdsTime = (Text*)btn_videoAds->getChildByName("text_time");

	for (auto i = 1; i <= 6; i++)
		clouds.add(rootNode->getChildByName(__String::createWithFormat("cloud_%d", i)->getCString()));
	cloudtime = rand() % 5;

	//AdsControl::getInstance()->showBannerAds();

}

float LevelMapScene::getOffsetForLevel(int index, int* pBgSegNo)
{
	float y = _hSum * (index / _nSum);
	index = index % _nSum;
	if (index == 0)
	{
		if (pBgSegNo != nullptr)
			*pBgSegNo = _bgSegments.getCount() - 1;
	}
	else
	{
		int i = _bgSegments.find([](BgSegment* b) { return b->getId() == 0; }, nullptr);
		for (; index > 0; i++)
		{
			if (i >= _bgSegments.getCount())
				i -= _bgSegments.getCount();
			if (index > _bgSegments[i]->getLevelCount())
			{
				y += _bgSegments[i]->getHeight();
				index -= _bgSegments[i]->getLevelCount();
			}
			else
			{
				y += _bgSegments[i]->getButtonPosY(index - 1);
				index = 0;
				if (pBgSegNo != nullptr)
					*pBgSegNo = i;
			}
		}
	}
	return y;
}

#define LEVELNUM_PER_PAGE	20

void LevelMapScene::refresh(bool reset)
{
	//calc scroll size
	int topLevel = UserData::getInstance()->getTopLevel();

	topLevel = ((topLevel - 1) / LEVELNUM_PER_PAGE) * LEVELNUM_PER_PAGE + (LEVELNUM_PER_PAGE);
	if (topLevel >= _levelManager->getLevelCount())
	{
		topLevel = _levelManager->getLevelCount();
	}
	else
	{
		_maplock->setVisible(true);
	}
	if (topLevel>_levelManager->getLevelCount() - LEVELNUM_PER_PAGE)
		_maplock->loadTexture("res/texture/Map/cloud_comingsoon.png");
	int hmax = getOffsetForLevel(topLevel) + _bottomHeight;
	_scrollView->setContentSize(Size(SCREEN_X, hmax + SCREEN_Y / 2));
	_maplock->setPosition(Vec2(SCREEN_X / 2, hmax + _maplock->getContentSize().height / 2));

	//centerize current level and update stars
	int index;
	if(_levelManager->getCurrentLevel() == nullptr)
	{
		index = UserData::getInstance()->getTopLevel();
	}
	else
	{
		index = LevelController::getInstance()->getCurrentLevel()->getLevelNumber();
	}
	if (index < 0 || index > _levelManager->getLevelCount()) index = UserData::getInstance()->getTopLevel();
	gotoLevel(index);

	for (int i = 0; i < _bgSegments.getCount(); i++)
		_bgSegments[i]->refresh(reset);
}

void LevelMapScene::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{
	auto layout = view->getContainer();
	float currentY = layout->getPositionY();

	if (currentY > 0) {
		view->setContentOffset(Vec2(0, 0));
	}
	if (-currentY > layout->getContentSize().height - view->getViewSize().height) {
		//view->setContentOffset(Vec2(0, -layout->getContentSize().height + view->getViewSize().height));
		_maplock->stopAllActions();
		//_maplock->setOpacity(125);
		_maplock->runAction(FadeTo::create(0.3, 125));
	}
	else {
		_maplock->stopAllActions();
		_maplock->setOpacity(255);
	}


	float offsetY = view->getContentOffset().y;
	float diff = _prevOffsetY - offsetY;

	float margin = _hSum / 3 - _bottomHeight;
	if (diff > 0)//scroll up
	{
		while (true)
		{
			if (diff < margin)
				break;
			auto first = _bgSegments[0];
			auto last = _bgSegments.getLast();
			_bgSegments.removeAt(0);
			_bgSegments.add(first);
			auto h = first->getHeight();
			first->setPosY(last->getTopY() + h / 2);
			first->setStartLevel(last->getLastLevel() + 1);
			first->refresh();
			diff -= h;
			_prevOffsetY -= h;
		}
	}
	else if (diff < 0)//scroll down
	{
		while (true)
		{
			if (-diff < margin)
				break;
			auto first = _bgSegments[0];
			if (first->getFirstLevel() == 1)
				break;
			auto last = _bgSegments.getLast();
			_bgSegments.removeLast();
			_bgSegments.insert(last, 0);
			auto h = last->getHeight();
			last->setPosY(first->getBottomY() - h / 2);
			last->setStartLevel(first->getFirstLevel() - last->getLevelCount());
			last->refresh();
			diff += h;
			_prevOffsetY += h;
		}
	}
}

void LevelMapScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{

		Director::getInstance()->replaceScene(TransitionFade::create(0.6f, StartScene::createScene()));
		event->stopPropagation();
	}
}

void LevelMapScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	SoundController::getInstance()->playBgMusic(SoundController::musicGameMenu);

	// keyboard
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(LevelMapScene::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	setGoldLabel();

	int topLevel = UserData::getInstance()->getTopLevel();
	auto absDays = UserData::getInstance()->getLastRateAbsDays();
	if (topLevel > 20 && absDays >= 7 && !UserData::getInstance()->isRateClickOk()) {
		//auto dlg = RateDialog::create();
		//dlg->btn_close->addClickEventListener([dlg](Ref*) {
		//	//SoundController::playEffectSound(SoundController::SoundEffect::sound_game_buttonclick);
		//	dlg->close();
		//});
		//dlg->btn_rate->addClickEventListener([dlg](Ref*) {
		//	//SoundController::playEffectSound(SoundController::SoundEffect::sound_game_buttonclick);
		//	dlg->close();
		//	UserData::getInstance()->clickRateOk();
		//	//GGBridge::rateApp();
		//});

		//dlg->show(this, kZPopup);
		//UserData::getInstance()->setRateAbsDays();
	}

	if (LevelController::getInstance()->getCurrentLevel()>0) {
		moveHeaderTo(UserData::getInstance()->getTopLevel(), 1);
	}
	else {
		moveHeaderTo(UserData::getInstance()->getTopLevel());
	}
	//setDailyState();
	//setVideoAdsButton();
}
void LevelMapScene::onExit() {
	this->unschedule(CC_SCHEDULE_SELECTOR(LevelMapScene::cloud_thread));
	Layer::onExit();
}
void LevelMapScene::onEnter()
{

	refresh(true);
	this->schedule(CC_SCHEDULE_SELECTOR(LevelMapScene::cloud_thread), 1.0f);
	Layer::onEnter();

}


void LevelMapScene::showSettingDlg()
{
	auto dlg = SettingDialog::create();

	auto soundController = SoundController::getInstance();
	auto imgMusicOn = dlg->btn_music->getChildByName("music_on");
	auto imgMusicOff = dlg->btn_music->getChildByName("music_off");
	auto imgSoundOn = dlg->btn_sound->getChildByName("sound_on");
	auto imgSoundOff = dlg->btn_sound->getChildByName("sound_off");

	if (soundController->isBgMusicOn())
	{
		imgMusicOn->setVisible(true);
		imgMusicOff->setVisible(false);
	}
	else
	{
		imgMusicOn->setVisible(false);
		imgMusicOff->setVisible(true);
	}

	if (soundController->isEffectsOn())
	{
		imgSoundOn->setVisible(true);
		imgSoundOff->setVisible(false);
	}
	else
	{
		imgSoundOn->setVisible(false);
		imgSoundOff->setVisible(true);
	}


	dlg->btn_exit->addClickEventListener([this, dlg](Ref*) {
		Director::getInstance()->replaceScene(TransitionFade::create(0.6f, StartScene::createScene()));
	});
	dlg->btn_continue->setVisible(false);
	dlg->btn_close->addClickEventListener([this, dlg](Ref*) {
		dlg->close();
		BoardController::gameState = Idle;
	});
	auto label = static_cast<ui::Text*>(dlg->btn_retry->getChildByName("lbl"));
	label->setString("Close");
	dlg->btn_retry->addClickEventListener([this, dlg](Ref*) {
		dlg->close();
		BoardController::gameState = Idle;
	});

	dlg->btn_music->addClickEventListener([=](Ref*) {
		soundController->toggleBgMusicOn();
		if (soundController->isBgMusicOn())
		{
			imgMusicOn->setVisible(true);
			imgMusicOff->setVisible(false);
		}
		else
		{
			imgMusicOn->setVisible(false);
			imgMusicOff->setVisible(true);
		}
	});
	dlg->btn_sound->addClickEventListener([=](Ref*) {
		soundController->toggleEffectsOn();
		if (soundController->isEffectsOn())
		{
			imgSoundOn->setVisible(true);
			imgSoundOff->setVisible(false);
		}
		else
		{
			imgSoundOn->setVisible(false);
			imgSoundOff->setVisible(true);
		}
	});
	dlg->setOnCloseHandler([this, dlg]() {
		BoardController::gameState = Idle;
	});
	dlg->retain();
	BoardController::gameState = Paused;
	dlg->show(this, kZPopup);
}

void LevelMapScene::onSelectLevel(Ref* pSender)
{
	int index = ((Node*)pSender)->getTag();
	GameController::getInstance()->goGamePlay(index);
}

void LevelMapScene::startLevel(int index)
{
	if (_levelManager->setCurrentLevel(index))
	{
		Director::getInstance()->pushScene(TransitionFade::create(1.0f, GamePlayScene::createScene()));
	}
}
Vec2 LevelMapScene::getPositionForLevel(int index, int* pBgSegNo) {
	float y = _hSum * (index / _nSum);
	index = index % _nSum;
	float x = 0;
	if (index == 0)
	{
		if (pBgSegNo != nullptr)
			*pBgSegNo = _bgSegments.getCount() - 1;
	}
	else
	{
		int i = _bgSegments.find([](BgSegment* b) { return b->getId() == 0; }, nullptr);
		for (; index > 0; i++)
		{
			if (i >= _bgSegments.getCount())
				i -= _bgSegments.getCount();
			if (index > _bgSegments[i]->getLevelCount())
			{
				int yy = _bgSegments[i]->getHeight();
				y += yy;
				index -= _bgSegments[i]->getLevelCount();
			}
			else
			{
				int yy = _bgSegments[i]->getButtonPosY(index - 1);
				x = _bgSegments[i]->getButtonPosX(index - 1);
				y += yy;

				index = 0;
				if (pBgSegNo != nullptr)
					*pBgSegNo = i;
			}
		}
	}
	return Vec2(x, y);
}
void LevelMapScene::gotoLevel(int index)
{
	float newOffset = getOffsetForLevel(index) + _bottomHeight;
	newOffset -= SCREEN_Y / 3;
	if (newOffset < 0)
		newOffset = 0;
	else if (newOffset > _scrollView->getContentSize().height - SCREEN_Y)
		newOffset = _scrollView->getContentSize().height - SCREEN_Y;

	float diff = _prevOffsetY + newOffset;
	if (diff > _hSum)
	{
		int r = (int)(diff / _hSum);
		float h = _hSum * r;
		int n = _nSum * r;
		for (int i = 0; i < _bgSegments.getCount(); i++)
		{
			auto seg = _bgSegments[i];
			seg->setPosY(seg->getPosY() + h);
			seg->setStartLevel(seg->getStartLevel() + n);
		}
		_prevOffsetY -= h;
	}

	_scrollView->setContentOffset(Vec2(0, -newOffset));
}
void LevelMapScene::moveHeaderTo(int index, int beforelevel)
{
	if (beforelevel == 0) {
		Vec2 pos = getPositionForLevel(index);
		pos.y = pos.y + _bottomHeight + 38;
		//pos.x = pos.x + origin.x;
		mHeader->stopAllActions();
		mHeader->setPosition(pos);
		auto act = EaseSineInOut::create(MoveTo::create(0.8f, Vec2(mHeader->getPosition().x, mHeader->getPosition().y + 5)));
		auto act1 = EaseSineInOut::create(MoveTo::create(0.8f, Vec2(mHeader->getPosition().x, mHeader->getPosition().y)));
		mHeader->runAction(RepeatForever::create(Sequence::create(act, act1, NULL)));
	}
	else {
		Vec2 pos = getPositionForLevel(index);
		pos.y = pos.y + _bottomHeight + 38;
		//pos.x = pos.x + origin.x;

		/*Vec2 oldpos = getPositionForLevel(beforelevel);
		oldpos.y = oldpos.y + _bottomHeight + 38;
		oldpos.x = oldpos.x + origin.x;*/

		mHeader->stopAllActions();
		auto move = MoveTo::create(1, pos);

		mHeader->runAction(Sequence::create(move,
			CallFunc::create([&]() {
			//g_trans_scene = 0;
		}),
			DelayTime::create(0.1f),
			CallFunc::create([&]() {
			auto act = EaseSineInOut::create(MoveTo::create(0.8f, Vec2(mHeader->getPosition().x, mHeader->getPosition().y + 5)));
			auto act1 = EaseSineInOut::create(MoveTo::create(0.8f, Vec2(mHeader->getPosition().x, mHeader->getPosition().y)));
			mHeader->runAction(RepeatForever::create(Sequence::create(act, act1, NULL)));
		}),
			NULL));

	}
}
void LevelMapScene::cloud_thread(float dt) {
	if(clouds.getCount() == 0)
	{
		return;
	}
	cloudtime -= dt;
	if (cloudtime <= 0) {
		List<Node*> flyclouds;
		for (int i = 0; i < 2; i++) {
			auto c = clouds.getAt(rand() % clouds.getCount());
			if (!flyclouds.contains(c) && c->getNumberOfRunningActions() == 0)
				flyclouds.add(c);
		}
		for (int i = 0; i < flyclouds.getCount(); i++) {
			auto c = flyclouds.getAt(i);
			int targetX = -c->getContentSize().width / 2;
			int dy = rand() % 100;

			int targetY = c->getPositionY() + (rand() % 2 == 0 ? dy : -dy);
			if (targetY > SCREEN_Y - 300 || targetY < 300)
				targetY = c->getPositionY();

			if (c->getPositionX() < 0) {
				targetX = SCREEN_X + c->getContentSize().width / 2;
			}

			c->runAction(MoveTo::create(5 + rand() % 5, Vec2(targetX, targetY)));

		}
		cloudtime = 5 + rand() % 5;
	}
}

void LevelMapScene::setVideoAdsButton() {
	UserData::getInstance()->calVideoAds();
	unschedule(CC_SCHEDULE_SELECTOR(LevelMapScene::updateVideoAdsTime));
	mTextVideoAdsTime->getParent()->stopAllActions();
	if (UserData::getInstance()->getVideoAdsTimes() <= 0) {
		mTextVideoAdsTime->setVisible(false);
		mTextVideoAdsTime->getParent()->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.8f, 1.2f), ScaleTo::create(0.8f, 1.0))));
	}
	else {
		mTextVideoAdsTime->setVisible(true);
		schedule(CC_SCHEDULE_SELECTOR(LevelMapScene::updateVideoAdsTime), 1);
	}
}
void LevelMapScene::updateVideoAdsTime(float ca) {
	long dt = UserData::getInstance()->diminishedVideoAdsTime();
	refreshVideoAdsTime();
	if (dt <= 0) {
		dt = 0;
		unschedule(CC_SCHEDULE_SELECTOR(LevelMapScene::updateVideoAdsTime));
		setVideoAdsButton();
	}
}
void LevelMapScene::refreshVideoAdsTime() {
	long dt = UserData::getInstance()->getVideoAdsTimes();
	mTextVideoAdsTime->setString(__String::createWithFormat("%02d:%02d",
		int((dt % 3600) / 60), static_cast<int>(dt % 60))->getCString());
}
void LevelMapScene::setDailyState() {
	UserData::getInstance()->calDaily();
	unschedule(CC_SCHEDULE_SELECTOR(LevelMapScene::updateDailyTime));
	mTextDailyTime->getParent()->stopAllActions();
	if (UserData::getInstance()->isBonus()) {
		mTextDailyTime->setVisible(false);
		mTextDailyTime->getParent()->runAction(RepeatForever::create(Sequence::createWithTwoActions(ScaleTo::create(0.8f, 1.2f), ScaleTo::create(0.8f, 1.0))));
	}
	else {
		mTextDailyTime->setVisible(true);
		refreshDailyTime();
		schedule(CC_SCHEDULE_SELECTOR(LevelMapScene::updateDailyTime), 1);
	}
}

void LevelMapScene::updateDailyTime(float ca) {
	long dt = UserData::getInstance()->diminishedDailyTime();
	refreshDailyTime();
	if (dt <= 0) {
		dt = 0;
		unschedule(CC_SCHEDULE_SELECTOR(LevelMapScene::updateDailyTime));
		setDailyState();
	}
}
void LevelMapScene::setGoldLabel()
{
	mGoldLabel->setString(StringUtils::toString(UserData::getInstance()->getGold()));
}
void LevelMapScene::refreshDailyTime() {
	long dt = UserData::getInstance()->getDailyTime();
	mTextDailyTime->setString(__String::createWithFormat("%02d:%02d:%02d",
		int(dt / 3600), int((dt % 3600) / 60), int(dt % 60))->getCString());
}


#pragma endregion
