#include "Dialogs.h"
#include "Models/DataModels/Level.h"
#include "Models/DataModels/UserData.h"
//#include "UI/BoosterPot.h"
#include "Scenes/GamePlayScene.h"
//#include "SoundManager.h"
//#include "Controllers/BoardController.h"
#include "cocostudio/CocoStudio.h"
#include "spine/spine-cocos2dx.h"
#include "Scenes/LevelMapScene.h"
#include "Native/AdsControl.h"
#include "Native/GGBridge.h"

using namespace spine;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;
using namespace cocos2d::ui;

static const char* gSelectedBoosterSpriteNames[3] = {
	"res/ui/Booster/boost_test_1.png",
	"res/ui/Booster/boost_test_2.png",
	"res/ui/Booster/boost_test_4.png",
};

LevelStartDialog* LevelStartDialog::createWithLevel(Level* level, float life)
{
    LevelStartDialog *pRet = new LevelStartDialog();
	if (pRet && pRet->initWithLevel(level, life))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

LevelStartDialog::LevelStartDialog()
{
	_life = 0;
}

bool LevelStartDialog::initWithLevel(Level* level, float life)
{
	Popup::initWithMask(false);

	_life = life;

	//auto spriteFrameCache = SpriteFrameCache::getInstance();
	//spriteFrameCache->addSpriteFramesWithFile("img_ig_ui.plist");

	//Size sz = Director::getInstance()->getWinSize();

	//auto bg = Sprite::createWithSpriteFrameName("ui_gift2.png");
	//bg->setPosition(sz / 2);
	//addChild(bg);


	//Size targetSize(75, 75);
	//Point orgPos(sz.width / 2 - targetSize.width * (level->getTargetCount() - 1) / 2, sz.height / 2 + 7);
	//for (int i = 0; i < level->getTargetCount(); i++)
	//{
	//	Point pos(orgPos.x + targetSize.width * i, orgPos.y);
	//	LevelTarget& target = level->getTarget(i);
	//	auto sprite = Sprite::createWithSpriteFrameName(gTargetSpriteNames[target.getType()]);
	//	sprite->setPosition(pos);
	//	addChild(sprite);
	//	auto count = Label::createWithBMFont("fnt_bomb.fnt", String::createWithFormat("%d", target.getCount())->getCString(), TextHAlignment::CENTER);
	//	count->setPosition(pos - Point(0, targetSize.height * .5f));
	//	//count->setColor(Color3B(33,39,122));
	//	count->setScale(.55f);
	//	addChild(count);
	//}

	return true;
}

void LevelStartDialog::onShow()
{
	scheduleOnce(schedule_selector(LevelStartDialog::onTime), _life);
}

void LevelStartDialog::onTime(float dt)
{
	close();
}

BoosterConfirmDialog* BoosterConfirmDialog::createWithBooster(BoosterPot* pot)
{
    BoosterConfirmDialog *pRet = new BoosterConfirmDialog();
	if (pRet && pRet->initWithBooster(pot))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool BoosterConfirmDialog::initWithBooster(BoosterPot* pot)
{
	Popup::initWithMask(false);

	//auto spriteFrameCache = SpriteFrameCache::getInstance();
	//spriteFrameCache->addSpriteFramesWithFile("img_ig_boost.plist");

	//Size sz = Director::getInstance()->getWinSize();

	//auto bg = Sprite::create("bg_ui_gift2.png");
	//bg->setPosition(sz / 2);
	//bg->setScale(469.f / bg->getContentSize().width, 304.f / bg->getContentSize().height);
	//addChild(bg);
	//
	//auto booster = Sprite::createWithSpriteFrameName(gBoosterFilenames[pot->getType()]);
	//booster->setPosition(sz.width / 2, sz.height / 2 + 47);
	//booster->setScale(2.4f);
	//addChild(booster);

	//auto sprite = Sprite::create("btn_pause_bj.png");
	//auto btn = MenuItemSprite::create(sprite, nullptr, nullptr, [this](Ref*) { close(); });
	//btn->setPosition(sz.width / 2, sz.height / 2 - 83);
	//btn->setScale(242.f / 186);
	//auto ok = Sprite::create("btn_ok.png");
	//ok->setPosition(sprite->getContentSize() / 2);
	//ok->setScale(0.7f);
	//btn->addChild(ok);
	//auto menu = Menu::create(btn, nullptr);
	//menu->setPosition(0, 0);
	//addChild(menu);

	return true;
}

OpenLevelDialog* OpenLevelDialog::createWithLevel(Level* level)
{
	OpenLevelDialog *pRet = new OpenLevelDialog();
	if (pRet && pRet->initWithLevel(level))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

OpenLevelDialog::OpenLevelDialog()
{
	_level = nullptr;
	_panel = nullptr;
	
}

bool OpenLevelDialog::initWithLevel(Level* level)
{
	Popup::initWithMask(true);

	_level = level;

	auto dlg = CSLoader::getInstance()->createNode("res/LevelOpenDialog.csb");
	addChild(dlg);

	rootNode = dlg->getChildByName("rootNode");
	btn_play = (Button*)rootNode->getChildByName("startButton");
	auto lbl_level = (Text*)rootNode->getChildByName("title")->getChildByName("title");
	////TextBMFont* lbl_level = (TextBMFont*)dlg->getChildByName("lbl_level");
	lbl_level->setString(__String::createWithFormat("Level %d", _level->getLevelNumber())->getCString());
	auto lblMessage = (Text*)rootNode->getChildByName("message");

	auto imagePath = rootNode->getChildByName("imagePath");
	imagePath->setVisible(false);
	auto imagePopsicle = rootNode->getChildByName("imagePopsicle");
	imagePopsicle->setVisible(false);
	auto imageThoppler = rootNode->getChildByName("imageThoppler");
	imageThoppler->setVisible(false);
	auto imageHider = rootNode->getChildByName("imageHider");
	imageHider->setVisible(false);
	auto imageWaffle = rootNode->getChildByName("imageWaffle");
	imageWaffle->setVisible(false);

	auto message = "";
	switch (_level->getLevelType())
	{
	case GoalTypes::WaffleObject:
		imageWaffle->setVisible(true);
		message = "Collecting Waffles";
		break;
	case GoalTypes::HiderSegmentObject:
		imageHider->setVisible(true);
		message = "Collecting Hiders";
		break;
	case GoalTypes::HopplingObject:
		imageThoppler->setVisible(true);
		imageThoppler->getChildByName("Toppler")->setVisible(false);
		imageThoppler->getChildByName("Hoppler")->setVisible(true);
		message = "Collecting Hopplers";
		break;
	case GoalTypes::PathObject:
		imagePath->setVisible(true);
		message = "Collecting Powers";
		break;
	case GoalTypes::PopsicleObject:
		imagePopsicle->setVisible(true);
		message = "Collecting Popsicles";
		break;
	case GoalTypes::TopplingObject:
		imageThoppler->setVisible(true);
		imageThoppler->getChildByName("Toppler")->setVisible(true);
		imageThoppler->getChildByName("Hoppler")->setVisible(false);
		message = "Collecting Topplers";
		break;
	}
	lblMessage->setString(message);
	//auto levelType = 
	//Button* btn_close = (Button*)rootNode->getChildByName("btn_close");
	//btn_close->addClickEventListener([this](Ref*){
	//	SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
	//	this->close();
	//});

	//auto star1 = rootNode->getChildByName("ig_star")->getChildByName("star1");
	//auto star2 = rootNode->getChildByName("ig_star")->getChildByName("star2");
	//auto star3 = rootNode->getChildByName("ig_star")->getChildByName("star3");

	//int star = GameData::getInstance()->getLevelStar(_level->getIndex());

	//star1->setVisible(star >= 1);
	//star2->setVisible(star >= 2);
	//star3->setVisible(star >= 3);

	//_panel = rootNode->getChildByName("nd_panel");
	//_panel->setVisible(false);

	//btn_gift = (Button*)rootNode->getChildByName("btn_gift");

	//btn_gift->setLocalZOrder(2);
	//btn_gift->addClickEventListener(CC_CALLBACK_1(OpenLevelDialog::onClickGift, this));
	//
	//auto _gift = rootNode->getChildByName("nd_gift");
	//_gift->setVisible(false);

	//auto skeletonNode = SkeletonAnimation::createWithJsonFile("res/effect/liwu.json", "res/effect/liwu.atlas");
	//skeletonNode->setAnimation(0, "liwu", true);
	//skeletonNode->setName("giftanimation");
	//skeletonNode->setCompleteListener([skeletonNode](int trackIndex, int loopCount) {
	//});
	//skeletonNode->setPosition(Vec2(_gift->getPositionX(),_gift->getPositionY()- skeletonNode->getContentSize().height /2));	
	//rootNode->addChild(skeletonNode);

	//auto ccc = btn_gift->clone();
	//ccc->setPosition(btn_gift->getPosition());
	//ccc->addClickEventListener([this](Ref* node) {
	//	onClickGift(node);
	//});
	//rootNode->addChild(ccc);

	//btn_gift->setVisible(false);

	//Button* btn_play = (Button*)rootNode->getChildByName("btn_play");
	//btn_play->addClickEventListener([&](Ref*) {
	//	close();
	//	SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);		
	//	Director::getInstance()->pushScene(TransitionFade::create(1.0f, GamePlayScene::createScene()));
	//});
	//

	return true;
}

void OpenLevelDialog::onClickGift(Ref* pSender)
{
	//SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_openGift);

	//
	//btn_gift->setVisible(false);

	//_level->selectBoosters(2);
	//auto _giftAnimation=(SkeletonAnimation*)rootNode->getChildByName("giftanimation");
	//_giftAnimation->setVisible(false);
	//_giftAnimation->removeAllChildrenWithCleanup(true);

	//ParticleSystemQuad* p = ParticleSystemQuad::create("res/effect/giftstar.plist");
	////p->setBlendAdditive(false);
	////p->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	////p->setPositionType(ParticleSystem::PositionType::FREE);
	////p->setPosition(_giftAnimation->getPosition());
	//auto node = (Node*)pSender;
	//p->setAutoRemoveOnFinish(true);
	//p->setPosition(Vec2(node->getContentSize().width *0.5, node->getContentSize().height *0.5));
	//node->addChild(p,100);

	//float gap = 160;
	//List<BoosterType>* boosters = _level->getSelectedBoosters();
	//Point orgPos(_panel->getContentSize().width / 2 - gap * (boosters->getCount() - 1) / 2, _panel->getContentSize().height / 2);
	//for (int i = 0; i < boosters->getCount(); i++)
	//{
	//	Point pos(orgPos.x + gap * i, orgPos.y);
	//	auto sprite = Sprite::createWithSpriteFrameName(gSelectedBoosterSpriteNames[(*boosters)[i]]);
	//	sprite->setPosition(pos);
	//	sprite->setScale(1.3f);
	//	_panel->addChild(sprite);
	//}
	//_panel->setVisible(true);


	//AdsControl::delayFullAds(0.8);
}


LevelBoosterDialog::LevelBoosterDialog()
{
	_life = 0;
}

LevelBoosterDialog* LevelBoosterDialog::createWithLevel(Level* level, float life)
{
    LevelBoosterDialog *pRet = new LevelBoosterDialog();
	if (pRet && pRet->initWithLevel(level, life))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool LevelBoosterDialog::initWithLevel(Level* level, float life)
{
	Popup::initWithMask(false);

	_life = life;

	//auto dlg = CSLoader::createNode("res/LevelBoosterDlg.csb");
	//addChild(dlg);

	//auto nd = dlg->getChildByName("nd_booster");

	//

	//List<BoosterType>* boosters = level->getSelectedBoosters();
	//if (boosters->getCount() == 1)
	//{
	//	auto sprite = Sprite::create(gSelectedBoosterSpriteNames[(*boosters)[0]]);		
	//	sprite->setScale(1.8f);
	//	nd->addChild(sprite);
	//}
	//else if (boosters->getCount() == 2)
	//{
	//	auto sprite1 = Sprite::create(gSelectedBoosterSpriteNames[(*boosters)[0]]);
	//	sprite1->setPosition(-100, 0);
	//	sprite1->setScale(1.8f);
	//	nd->addChild(sprite1);
	//	auto sprite2 = Sprite::create(gSelectedBoosterSpriteNames[(*boosters)[1]]);
	//	sprite2->setPosition(100, 0);
	//	sprite2->setScale(1.8f);
	//	nd->addChild(sprite2);
	//}

	return true;
}

void LevelBoosterDialog::onShow()
{
	scheduleOnce(schedule_selector(LevelBoosterDialog::onTime), _life);
}

void LevelBoosterDialog::onTime(float dt)
{
	close();
}

SettingDialog::SettingDialog()
{
	Popup::initWithMask(true);

	auto dlg = CSLoader::getInstance()->createNode("res/SettingDlg.csb");
	addChild(dlg);

	auto rootNode = dlg->getChildByName("rootNode");
	btn_close = (Button*)rootNode->getChildByName("btn_close");
	btn_music = (Button*)rootNode->getChildByName("btn_music");
	btn_sound = (Button*)rootNode->getChildByName("btn_sound");
	btn_exit = (Button*)rootNode->getChildByName("btn_home");
	btn_retry = (Button*)rootNode->getChildByName("btn_restart");
	mute_music = (ImageView*)btn_music->getChildByName("ig_mute");
	mute_sound = (ImageView*)btn_sound->getChildByName("ig_mute");
	btn_continue = (Button*)rootNode->getChildByName("btn_continue");


	//mute_music->setVisible(!SoundController::isBgMusicOn());
	//mute_sound->setVisible(!SoundController::isEffectsOn());

	//btn_music->addClickEventListener([this](Ref*){
	//	SoundManager::toggleBgMusicOn();
	//	SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
	//	mute_music->setVisible(!SoundManager::isBgMusicOn());
	//});
	//btn_sound->addClickEventListener([this](Ref*){
	//	SoundManager::toggleEffectsOn();
	//	SoundManager::playEffectSound(SoundManager::SoundEffect::sound_game_buttonclick);
	//	mute_sound->setVisible(!SoundManager::isEffectsOn());
	//});
}

GameWinDialog::GameWinDialog()
{
	Popup::initWithMask(true);
	//TextureCache::sharedTextureCache()->addImage("particle/particle_yuanhua.png");

	auto dlg = CSLoader::getInstance()->createNode("res/FinishDlg.csb");
	addChild(dlg); 

	auto rootNode = dlg->getChildByName("com");
	m_pUiNode = rootNode;
	btn_close = (Button*)rootNode->getChildByName("Image_2");
	btn_next = (Button*)rootNode->getChildByName("button_1");
	//lbl_level = (Text*)rootNode->getChildByName("ig_title")->getChildByName("lbl_level");
	lbl_maxscore = (Text*)rootNode->getChildByName("text_5");
	lbl_maxscore_title = (Text*)rootNode->getChildByName("text_3");
	lbl_score = (Text*)rootNode->getChildByName("text_4");
	lbl_score_title = (Text*)rootNode->getChildByName("text_2");

	m_rewardedCoin = rootNode->getChildByName("ig_coin");
	
	
	star1 = (Sprite*)rootNode->getChildByName("image_13")->getChildByName("image_14");
	star2 = (Sprite*)rootNode->getChildByName("image_11")->getChildByName("image_12");
	star3 = (Sprite*)rootNode->getChildByName("image_15")->getChildByName("finish_star_3");


	star1->setVisible(false);
	star2->setVisible(false);
	star3->setVisible(false);	
}

void GameWinDialog::setStar(int count)
{
	if (count > 0)
	{
		float t = 0.5f;
		star1->runAction(Sequence::create(
			Show::create(),
			EaseSineIn::create(ScaleTo::create(t, 1)),
			nullptr));
		if (count > 1) {
			star2->runAction(Sequence::create(
				DelayTime::create(t),
				Show::create(),
				EaseSineIn::create(ScaleTo::create(t, 1)),
				nullptr));
			if (count > 2){
				star3->runAction(Sequence::create(
					DelayTime::create(t * 2),
					Show::create(),
					EaseSineIn::create(ScaleTo::create(t, 1)),
					nullptr));
			}
		}
		//if (count == 3) {

		//	for (int i = 0; i < 5; i++) {
		//		auto particle = (ParticleSystemQuad*)m_pUiNode->getChildByName(__String::createWithFormat("Particle_%d",i+1)->getCString());
		//		particle->stopSystem();				
		//		this->runAction(Sequence::create(
		//			DelayTime::create(0.1+i*t),
		//			CallFunc::create([&, particle]() {
		//			particle->setAutoRemoveOnFinish(true);
		//			particle->start();
		//			
		//		}),
		//			nullptr));
		//		
		//	}
		//}
	}
}

GameFailedDialog::GameFailedDialog()
{
	Popup::initWithMask(true);
	auto dlg = CSLoader::getInstance()->createNode("res/FailedDailogui.csb");
	addChild(dlg);

	btn_close = static_cast<Button*>(dlg->getChildByName("close"));
	btn_playon = static_cast<Button*>(dlg->getChildByName("playButton"));
	btn_video = static_cast<Button*>(dlg->getChildByName("videoButton"));
}


GameLoseDialog::GameLoseDialog()
{
	Popup::initWithMask(true);

	auto dlg = CSLoader::getInstance()->createNode("res/GameLoseDlg.csb");
	addChild(dlg);

	auto rootNode = dlg->getChildByName("rootNode");

	btn_close = (Button*)rootNode->getChildByName("btn_close");
	btn_retry = (Button*)rootNode->getChildByName("btn_play");
	lbl_level = (Text*)rootNode->getChildByName("ig_title")->getChildByName("lbl_level");
	lbl_score = (Text*)rootNode->getChildByName("lbl_score");
}

ExitGameDialog::ExitGameDialog()
{
	Popup::initWithMask(true);

	auto dlg = CSLoader::getInstance()->createNode("ExitGameDlg.csb");
	addChild(dlg);

	btn_back = (Button*)dlg->getChildByName("btn_back");
	btn_exit = (Button*)dlg->getChildByName("btn_exit");
}
RateDialog::RateDialog()
{
	
	Popup::initWithMask(true);

	auto dlg = CSLoader::getInstance()->createNode("res/RateDlg.csb");
	addChild(dlg);

	auto rootNode = dlg->getChildByName("rootNode");

	btn_close = (Button*)rootNode->getChildByName("btn_close");
	btn_rate = (Button*)rootNode->getChildByName("btn_rate");
}
ShopDialog* ShopDialog::_instance=nullptr;
void ShopDialog::resetShopState(int adType)
{
	auto rootNode = dlgRoot->getChildByName("rootNode");


	auto btnRoot = rootNode->getChildByName(__String::createWithFormat("ad_panel%d", adType + 1)->getCString());
	auto btnPlay = (Button*)btnRoot->getChildByName("btn_playads");


	auto image_total = (ImageView*)btnPlay->getChildByName("image_total");
	auto lbl_total = (Text*)image_total->getChildByName("lbl_total");
	auto todayAds = UserData::getInstance()->getTodayAvailableAds(adType);
	lbl_total->setString(StringUtils::toString(todayAds));
	if (todayAds <= 0) {
		btnPlay->setEnabled(false);
		image_total->stopAllActions();
	}
	else {
		btnPlay->setEnabled(true);
		image_total->stopAllActions();
		image_total->runAction(RepeatForever::create(Sequence::create(DelayTime::create(RandomHelper::random_real(0.5, 1.5)), ScaleTo::create(0.08, 1.2, 0.8),
			ScaleTo::create(0.08, 0.8, 1.2), ScaleTo::create(0.1, 1.15, 0.85),
			ScaleTo::create(0.1, 0.9, 1.1), ScaleTo::create(0.13, 1.1, 0.9),
			ScaleTo::create(0.13, 0.9, 1.1), ScaleTo::create(0.16, 1.05, 0.95),
			ScaleTo::create(0.16, 1, 1), nullptr)));
	}
}

ShopDialog::ShopDialog()
{
	_instance = this;
	Popup::initWithMask(true);
	setTouchMode(kCCTouchesOneByOne);
	dlgRoot = CSLoader::getInstance()->createNode("res/ShopDlg.csb");
	addChild(dlgRoot);

	auto rootNode = dlgRoot->getChildByName("rootNode");

	((Button*)rootNode->getChildByName("btn_close"))->addClickEventListener([this](Ref*) {
		this->close();
	});

	int goldCount[] = { REWARDED_VIDEO,REWARDED_ADS };

	for (int i = 1; i <= 2; i++) {
		auto btnRoot = rootNode->getChildByName(__String::createWithFormat("ad_panel%d", i)->getCString());
		((Text*)btnRoot->getChildByName("lbl_coin"))->setString(StringUtils::toString(goldCount[i - 1]));
		auto btnPlay = (Button*)btnRoot->getChildByName("btn_playads");
		btnPlay->setTag(i);
		int goldBuyCount = goldCount[i - 1];
		int adType = i - 1;
		btnPlay->addClickEventListener([this, goldBuyCount, adType](Ref*) {
			onClickBtnBuy(goldBuyCount, adType);
		});

		auto image_total = (ImageView*)btnPlay->getChildByName("image_total");
		auto lbl_total = (Text*)image_total->getChildByName("lbl_total");
		auto todayAds = UserData::getInstance()->getTodayAvailableAds(i - 1);
		lbl_total->setString(StringUtils::toString(todayAds));
		if (todayAds <= 0) {
			btnPlay->setEnabled(false);
			image_total->stopAllActions();
		}
		else {
			btnPlay->setEnabled(true);
			image_total->stopAllActions();
			image_total->runAction(RepeatForever::create(Sequence::create(DelayTime::create(RandomHelper::random_real(0.5, 1.5)), ScaleTo::create(0.08, 1.2, 0.8),
				ScaleTo::create(0.08, 0.8, 1.2), ScaleTo::create(0.1, 1.15, 0.85),
				ScaleTo::create(0.1, 0.9, 1.1), ScaleTo::create(0.13, 1.1, 0.9),
				ScaleTo::create(0.13, 0.9, 1.1), ScaleTo::create(0.16, 1.05, 0.95),
				ScaleTo::create(0.16, 1, 1), nullptr)));
		}
	}
}

void ShopDialog::onClickBtnBuy(int gCount, int adType)
{
	auto messageBox = MessageDialog::create();

	int nAds = UserData::getInstance()->getTodayAvailableAds(adType);
	if (nAds <= 0) {
		
		
		messageBox->setTitle("Error");
		messageBox->setMessage("Today you can not click on ads to get diamonds.");
		messageBox->show(this,2);

		return;
	}
	switch (adType) {
	case 1:
		if (GGBridge::hasInterstitialAd()) {
			AdsControl::delayFullAds(0);
			UserData::getInstance()->setTodayAdsClick(adType);
			UserData::getInstance()->changeGold(gCount);
			UserData::getInstance()->saveGold();
			LevelMapScene::getInstance()->setGoldLabel();

			/*if (OutOfMovesLayer::getInstance()) {
				OutOfMovesLayer::getInstance()->updateCoin();
			}*/
			if (BoosterBuyDialog::getInstance())
				BoosterBuyDialog::getInstance()->updateCoin();
		}
		else {
			messageBox->setTitle("Error");
			messageBox->setMessage("Ads not ready!");
			messageBox->show(this, 2);
			
		}
		break;
	case 0:
		if (GGBridge::hasRewardVideoAds()) {
			GGBridge::showRewardVideoAds();
			UserData::getInstance()->setTodayAdsClick(adType);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			rewardedVideoFinish();
#endif
		}
		else {
			messageBox->setTitle("Error");
			messageBox->setMessage("Ads not ready!");
			messageBox->show(this, 2);
		}
		break;
	default:break;
	}
	resetShopState(adType);
}
void ShopDialog::rewardedVideoFinish() {

	
	UserData::getInstance()->changeGold(REWARDED_VIDEO);
	UserData::getInstance()->saveGold();

	if (LevelMapScene::getInstance())
		LevelMapScene::getInstance()->setGoldLabel();

	/*if (OutOfMovesLayer::getInstance()) {
	OutOfMovesLayer::getInstance()->updateCoin();
	}*/
	if (BoosterBuyDialog::getInstance())
		BoosterBuyDialog::getInstance()->updateCoin();

}

MessageDialog::MessageDialog()
{
	Popup::initWithMask(true);
	//setTouchMode(kCCTouchesOneByOne);
	//auto dlg = CSLoader::getInstance()->createNode("res/MessageDlg.csb");
	//addChild(dlg);

	//auto rootNode = dlg->getChildByName("rootNode");

	//((Button*)rootNode->getChildByName("btn_close"))->addClickEventListener([this](Ref*) {
	//	this->close();
	//});
	//((Button*)rootNode->getChildByName("btn_ok"))->addClickEventListener([this](Ref*) {
	//	this->close();
	//});
	//text_ok = (Text*)rootNode->getChildByName("btn_ok")->getChildByName("text_ok");
	//text_ok->setString( "OK");
	//lbl_title = (Text*)rootNode->getChildByName("ig_title")->getChildByName("lbl_title");
	//lbl_info = (Text*)rootNode->getChildByName("lbl_info");
}
void MessageDialog::setTitle(const char* title) {
	lbl_title->setString(title);
}
void MessageDialog::setMessage(const char* message) {
	lbl_info->setString(message);
}
void MessageDialog::setAutoClose(bool allowAutoClose, float closeDelay ) {
	
}
BoosterBuyDialog* BoosterBuyDialog::_instance = nullptr;
BoosterBuyDialog::BoosterBuyDialog()
{
	_instance = this;
	nFee = 0;
	Popup::initWithMask(true);
	//setTouchMode(kCCTouchesOneByOne);
	auto dlgRoot = CSLoader::getInstance()->createNode("res/BoosterBuyDlg.csb");
	addChild(dlgRoot);

	this->setOnCloseHandler([this]() {
		this->close();
	});

	auto rootNode = dlgRoot->getChildByName("rootNode");

	((Button*)rootNode->getChildByName("btn_close"))->addClickEventListener([this](Ref*) {
		setOnCloseHandler(nullptr);
		this->close();
	});

	((Button*)rootNode->getChildByName("btn_buy"))->addClickEventListener([this](Ref*) {
		onClickBuy();
	});
	text_fee = (Text*)rootNode->getChildByName("btn_buy")->getChildByName("txt_fee");

	mImageBooster = (ImageView*)rootNode->getChildByName("img_boosterpic");

	Button* btn_coin = (Button*)rootNode->getChildByName("btn_coin");
	btn_coin->addClickEventListener([this](Ref*) {
		ShopDialog::create()->show(this,10);
		
	});

	Vec2 pos = btn_coin->getPosition();
	btn_coin->setPositionX(-btn_coin->getContentSize().width / 2);
	btn_coin->runAction(Sequence::create(DelayTime::create(0.5f), MoveTo::create(0.8, pos), NULL));

	text_coin = (Text*)btn_coin->getChildByName("lbl_coin");
	updateCoin();
}
BoosterBuyDialog::~BoosterBuyDialog()
{
	_instance = nullptr;
}
void BoosterBuyDialog::updateCoin() {
	text_coin->setString(StringUtils::toString(UserData::getInstance()->getGold()));
}
void BoosterBuyDialog::initWithBoosterNumber(BoosterType boosterType)
{
	nFee = 0;
	nBoosterNum = boosterType;
	switch (boosterType)
	{
	case BoosterType::BoosterHor:
		mImageBooster->loadTexture("banner_10.png", TextureResType::PLIST);
		nFee = FEE_BUYHOR;
		break;
	case BoosterType::BoosterVer:
		mImageBooster->loadTexture("banner_14.png", TextureResType::PLIST);
		nFee = FEE_BUYVER;
		break;
	case BoosterType::BoosterSingle:
		mImageBooster->loadTexture("banner_13.png", TextureResType::PLIST);
		nFee = FEE_BUYSINGLE;
		break;
	case BoosterType::BoosterSwap:
		mImageBooster->loadTexture("banner_11.png", TextureResType::PLIST);
		nFee = FEE_BUYSWAP;
		break;
	//case 4:
	//	mImageBooster->loadTexture("res/ui/Booster/boost_test_5.png");
	//	nFee = FEE_SAMECOLOR;
	//	break;
	}
	text_fee->setString(StringUtils::toString(nFee));
}

void BoosterBuyDialog::onClickBuy()
{
	int goldCount = nFee;
	
	if (UserData::getInstance()->getGold() >= goldCount)
	{
		UserData::getInstance()->changeGold(-goldCount);
		UserData::getInstance()->nBoosterCount[nBoosterNum] += 3;
		UserData::getInstance()->saveGold();
		UserData::getInstance()->saveBooster();
		GamePlayScene::getInstance()->updateBoosterCount();
		this->close();
	}
	else {
		ShopDialog::create()->show(this, 10);
	}
}
