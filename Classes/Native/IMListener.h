#pragma once
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#include <string>

static std::string kHomeBanner = "home";
static std::string kGameOverAd = "gameover";
static std::string kRewardedAd = "rewarded";

class IMListener : public sdkbox::AdMobListener {
public:
	virtual void adViewDidReceiveAd(const std::string &name) {
		cocos2d::log("%s name=%s", __FUNCTION__, name.c_str());
		if (name == kHomeBanner) sdkbox::PluginAdMob::show(name);
	}
	virtual void adViewDidFailToReceiveAdWithError(const std::string &name, const std::string &msg) {
		cocos2d::log("%s\nname=%s,\nmsg=%s", __FUNCTION__, name.c_str(), msg.c_str());
	}
	virtual void adViewWillPresentScreen(const std::string &name) {
		cocos2d::log("%s name=%s", __FUNCTION__, name.c_str());
	}
	virtual void adViewDidDismissScreen(const std::string &name) {
		cocos2d::log("%s name=%s", __FUNCTION__, name.c_str());
	}
	virtual void adViewWillDismissScreen(const std::string &name) {
		cocos2d::log("%s name=%s", __FUNCTION__, name.c_str());

		if (name == "gameover") {
			sdkbox::PluginAdMob::cache(kGameOverAd);
		}
		else if (name == kRewardedAd) {
			sdkbox::PluginAdMob::cache(kRewardedAd);
		}
	}
	virtual void adViewWillLeaveApplication(const std::string &name) {
		cocos2d::log("%s name=%s", __FUNCTION__, name.c_str());
	}
	virtual void reward(const std::string &name, const std::string &currency, double amount) {
		
		cocos2d::log("%s name=%s currency=%s amount=%lf",
			__FUNCTION__, name.c_str(), currency.c_str(), amount);
	}
};


#endif
