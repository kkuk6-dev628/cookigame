#include "UserData.h"
#include "General/Constants.h"

USING_NS_CC;

extern bool g_bDebugMode;
UserData* UserData::instance = nullptr;

UserData* UserData::getInstance()
{
	if (instance == nullptr)
	{
		instance = new UserData();
	}
	return instance;
}


UserData::UserData()
{
	userDefault = cocos2d::UserDefault::getInstance();
	_nGold = userDefault->getIntegerForKey("gold", 0);

	auto strTmp = UserDefault::getInstance()->getStringForKey("BoosterCount", "3,3,3,3,3");
	auto *arr = componentsSeparatedByString(__String::create(strTmp), ",");
	for (int i = 0; i < arr->count(); i++)
	{
		nBoosterCount[i] = static_cast<__String*>(arr->getObjectAtIndex(i))->intValue();
	}

	topLevel = userDefault->getIntegerForKey("top_level", 1);
	topLevel = MAX(topLevel, 130);

	//topLevel = 1599;//test
	//if (g_bDebugMode) topLevel = 45;
#ifdef TEST_UserDataClearOnInit
	clear();
#endif
}

UserData::~UserData()
{
	instance = nullptr;
}

void UserData::saveBooster() {
	std::string strTmp = "";
	for (int i = 0; i < BoosterCount; i++)
		strTmp += __String::createWithFormat("%d", nBoosterCount[i])->getCString();

	UserDefault::getInstance()->setStringForKey("BoosterCount", strTmp);
}
void UserData::clear()
{
	topLevel = 0;
	incTopLevel();
}

void UserData::incTopLevel()
{
	topLevel++;
	userDefault->setIntegerForKey("toplevel", topLevel);
}

void UserData::onLevelPass(int level, int score, int star)
{
	if (level == topLevel)
	{
		incTopLevel();
	}
	if (score > getLevelMaxScore(level))
		setLevelMaxScore(level, score);
	if (star > getLevelStar(level))
		setLevelStar(level, star);
	userDefault->flush();
}

int UserData::getLevelStar(int level)
{
	return userDefault->getIntegerForKey(__String::createWithFormat("star%d", level)->getCString(), 0);
}

void UserData::setLevelStar(int level, int count)
{
	if (count < 0) count = 0;
	else if (count > MAX_STAR) count = MAX_STAR;
	userDefault->setIntegerForKey(__String::createWithFormat("star%d", level)->getCString(), count);
}

int UserData::getLevelMaxScore(int level)
{
	return userDefault->getIntegerForKey(__String::createWithFormat("score%d", level)->getCString(), 0);
}

void UserData::setLevelMaxScore(int level, int score)
{
	userDefault->setIntegerForKey(__String::createWithFormat("score%d", level)->getCString(), score);
}

int UserData::setAndGetLevelMaxScore(int level, int score)
{
	const char* key = __String::createWithFormat("score%d", level)->getCString();
	int maxScore = userDefault->getIntegerForKey(key, 0);
	if (score > maxScore)
	{
		userDefault->setIntegerForKey(key, score);
		maxScore = score;
	}
	return maxScore;
}


__Array* UserData::componentsSeparatedByString(__String* srcStr, const char*sepStr)
{
	auto arr = __Array::create();
	std::string src = srcStr->getCString();
	std::string sep(sepStr);
	std::string subStr;
	int n1 = 0;
	int n2 = src.find(sep, n1);
	while (n2 >= 0)
	{
		subStr = src.substr(n1, n2 - n1);
		if (subStr != "")
			arr->addObject(__String::create(subStr));
		n1 = n2 + 1;
		n2 = src.find(sep, n1);
	}
	n2 = src.length();
	subStr = src.substr(n1, n2 - n1);
	if (subStr != "")
		arr->addObject(__String::create(subStr));

	return arr;
}


//bool GameState::_isPaused = false;
//bool GameState::_isEnded = false;
//bool GameState::_isBonusTime = false;
//bool GameState::_isEndedAnimation = false;

void UserData::calVideoAds() {
	long vtime = UserDefault::getInstance()->getIntegerForKey("videoAdsTime", 0);
	if (vtime == 0) {
		mVideoAdsTime = 0;
		return;
	}
	long cur_time = time(nullptr);
	mVideoAdsTime = REWARDED_VIDEO_DEFAULT_TIME - (cur_time - vtime);
	if (mVideoAdsTime > REWARDED_VIDEO_DEFAULT_TIME)
		mVideoAdsTime = REWARDED_VIDEO_DEFAULT_TIME;
	if (mVideoAdsTime < 0)
		mVideoAdsTime = 0;
}
void UserData::videoAdsWatch() {
	UserDefault::getInstance()->setIntegerForKey("videoAdsTime", time(nullptr));
	mVideoAdsTime = REWARDED_VIDEO_DEFAULT_TIME;
}
void UserData::calDaily()
{
	long daily_time = UserDefault::getInstance()->getIntegerForKey("Daily0", 0);
	mDailyOrder = UserDefault::getInstance()->getIntegerForKey("Daily_Number", 0);

	if (daily_time <= 0) {
		bBonusFlag = true;
		mDailyOrder = 1;
		mDailyTime = 0;
		return;
	}
	else {
		long cur_time = time(nullptr);
		long d_time = cur_time - daily_time;
		if (d_time > 86400 * 2) {
			mDailyOrder = 1;
			bBonusFlag = true;
		}
		else if (d_time >= 86400 && d_time < 86400 * 2) {
			mDailyOrder++;
			bBonusFlag = true;
			if (mDailyOrder > 9)
				mDailyOrder = 1;
		}
		else if (d_time < 86400)
			bBonusFlag = false;

		mDailyTime = 86400 - (cur_time - daily_time) % 86400;
	}
}

int UserData::getLastRateAbsDays() {
	auto d = UserDefault::getInstance()->getDataForKey("time_rate", Data());
	if (d.isNull()) {
		return 7;
	}
	else {
		time_t lastExitTime = *((time_t*)d.getBytes());
		time_t currentTime = time(0);
		time_t time = currentTime - lastExitTime;

		return time / 60 / 60 / 24;
	}
}
void UserData::setRateAbsDays() {
	time_t t = time(0);
	Data data;
	data.copy((unsigned char *)&t, sizeof(t));
	UserDefault::getInstance()->setDataForKey("time_rate", data);
}
bool UserData::isRateClickOk() {
	return UserDefault::getInstance()->getBoolForKey("rateok", false);
}
void UserData::clickRateOk() {
	UserDefault::getInstance()->setBoolForKey("rateok", true);
}


#define DAILYTATOL_ADS 5
int UserData::getTodayAvailableAds(int type) {
	long nDaily_time = UserDefault::getInstance()->getIntegerForKey(__String::createWithFormat("DailyAds_%d", type)->getCString(), 0);
	int nDaily_Total = UserDefault::getInstance()->getIntegerForKey(__String::createWithFormat("DailyTotalAds_%d", type)->getCString(), 0);


	if (nDaily_time <= 0) {
		nDaily_Total = DAILYTATOL_ADS;
	}
	else {
		long cur_time = getTodayTimeStamp();
		long d_time = cur_time - nDaily_time;
		if (d_time >= 86400) {
			nDaily_Total = DAILYTATOL_ADS;
		}
	}
	if (nDaily_Total < 0) nDaily_Total = 0;
	return nDaily_Total;
}
void UserData::setTodayAdsClick(int type) {
	long cur_time = time(nullptr);
	long nDaily_time = UserDefault::getInstance()->getIntegerForKey(__String::createWithFormat("DailyAds_%d", type)->getCString(), 0);
	int nDaily_Total = getTodayAvailableAds(type);
	long c_time = getTodayTimeStamp();
	nDaily_Total--;
	UserDefault::getInstance()->setIntegerForKey(__String::createWithFormat("DailyAds_%d", type)->getCString(), c_time);
	UserDefault::getInstance()->setIntegerForKey(__String::createWithFormat("DailyTotalAds_%d", type)->getCString(), nDaily_Total);

}
