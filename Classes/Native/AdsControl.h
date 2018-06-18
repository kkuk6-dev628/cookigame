#ifndef __ADSCONTROL_____
#define __ADSCONTROL_____

#include "cocos2d.h"


USING_NS_CC;


class AdsControl 
{
public:	

	static AdsControl* getInstance();
	void showInterstitialAds();
	void showRewardedVideoAds(bool buyCoins = true);
	void showBannerAds();

	bool isAvailableInterstitialAds();
	bool isAvailableRewardedVideoAds();
	bool isAvailableBannerAds();

private:
	AdsControl();
	static AdsControl* instance;
};


#endif