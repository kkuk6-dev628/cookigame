#pragma once
#include "cocos2d.h"
#include "General/EnumTypes.h"

class UserData
{
public:
	static UserData* getInstance();
	~UserData();

private: 
	UserData();

private:
	cocos2d::UserDefault* userDefault;
	static UserData* instance;
	int topLevel;


#pragma region Get Set Method
public:

	int getTopLevel() const
	{
		return topLevel;
	}

	void setTopLevel(int topLevel)
	{
		if(this->topLevel < topLevel)
		{
			this->topLevel = topLevel;
			userDefault->setIntegerForKey("top_level", topLevel);
			//userDefault->flush();
		}
	}

#pragma endregion

	void onLevelPass(int level, int score, int star);

	int getLevelStar(int level);
	void setLevelStar(int level, int count);
	int getLevelMaxScore(int level);
	void setLevelMaxScore(int level, int score);
	int setAndGetLevelMaxScore(int level, int score);



	int getGold() { return _nGold; };
	void changeGold(int vel) { _nGold += vel; };
	void saveGold() { userDefault->setIntegerForKey("gold", _nGold); };

	void calDaily();
	int getDailyOrder() { return mDailyOrder; }
	long getDailyTime() { return mDailyTime; }
	long diminishedDailyTime() {
		mDailyTime--;
		if (mDailyTime < 0) {
			mDailyTime = 0;
		}
		return mDailyTime;
	};
	bool isBonus() { return bBonusFlag; }


	void calVideoAds();
	long getVideoAdsTimes() { return mVideoAdsTime; }
	long diminishedVideoAdsTime() {
		mVideoAdsTime--;
		if (mVideoAdsTime < 0) {
			mVideoAdsTime = 0;
		}
		return mVideoAdsTime;
	};
	void videoAdsWatch();

	int getLastRateAbsDays();
	void setRateAbsDays();
	bool isRateClickOk();
	void clickRateOk();


	int nBoosterCount[BoosterCount];
	void saveBooster();

	int getTodayAvailableAds(int type = 0);
	void setTodayAdsClick(int type = 0);
	cocos2d::__Array* componentsSeparatedByString(cocos2d::__String* srcStr, const char*sepStr);
	static long getTodayTimeStamp() {
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		timeinfo->tm_hour = 0;
		timeinfo->tm_min = 0;
		timeinfo->tm_sec = 0;
		return mktime(timeinfo);
	}


private:
	int mDailyOrder;
	long mDailyTime;
	bool bBonusFlag;

	long mVideoAdsTime;
private:
	void clear();
	void incTopLevel();

	int	_nGold;
};

//class GameState
//{
//public:
//	static void onStart() { _isPaused = false; _isEnded = false; _isBonusTime = false; }
//	static void onPause() { _isPaused = true; }
//	static void onResume() { _isPaused = false; }
//	static void onEnded() { _isEnded = true; }
//
//	static bool isPaused() { return _isPaused; }
//	static bool isEnded() { return _isEnded; }
//
//	static void onStartBonusTime() { _isBonusTime = true; }
//	static void onEndBonusTime() { _isBonusTime = false; }
//	static bool isBonusTime() { return _isBonusTime; }
//
//	static void onStartEndedAnimation() { _isEndedAnimation = true; }
//	static void onEndEndedAnimation() { _isEndedAnimation = false; }
//	static bool isEndedAnimation() { return _isEndedAnimation; }
//
//private:
//	static bool _isPaused;
//	static bool _isEnded;
//	static bool _isBonusTime;
//	static bool _isEndedAnimation;
//};

