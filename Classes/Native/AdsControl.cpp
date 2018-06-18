#include "AdsControl.h"
#include "GGBridge.h"
#include "Native/IMListener.h"
#include "Layers/Dialogs.h"

#ifdef SDKBOX_ENABLED

class BuyCoinsAdMobListener : public IMListener
{
	virtual void reward(const std::string &name, const std::string &currency, double amount) override {

		ShopDialog::getInstance()->rewardedVideoFinish();

	}
};

#endif

AdsControl* AdsControl::instance = nullptr;

AdsControl* AdsControl::getInstance()
{
	if(instance == nullptr)
	{
		instance = new AdsControl;
	}
	return instance;
}

void AdsControl::showInterstitialAds()
{
#ifdef SDKBOX_ENABLED
	if(sdkbox::PluginAdMob::isAvailable(kGameOverAd))
		sdkbox::PluginAdMob::show(kGameOverAd);
#endif
}

void AdsControl::showRewardedVideoAds(bool buyCoins)
{
#ifdef SDKBOX_ENABLED
	if(buyCoins)
	{
		sdkbox::PluginAdMob::setListener(new BuyCoinsAdMobListener());
	}
	else
	{
		sdkbox::PluginAdMob::setListener(new IMListener());
	}
	if (sdkbox::PluginAdMob::isAvailable(kRewardedAd))
		sdkbox::PluginAdMob::show(kRewardedAd);
#endif
}

void AdsControl::showBannerAds()
{
#ifdef SDKBOX_ENABLED
	if (sdkbox::PluginAdMob::isAvailable(kHomeBanner))
		sdkbox::PluginAdMob::show(kHomeBanner);
#endif
}

AdsControl::AdsControl()
{
#ifdef SDKBOX_ENABLED
	sdkbox::PluginAdMob::setListener(new IMListener());
#endif
}

bool AdsControl::isAvailableInterstitialAds()
{
#ifdef SDKBOX_ENABLED
	return sdkbox::PluginAdMob::isAvailable(kGameOverAd);
#endif
	return false;
}

bool AdsControl::isAvailableRewardedVideoAds()
{
#ifdef SDKBOX_ENABLED
	return sdkbox::PluginAdMob::isAvailable(kRewardedAd);
#endif
	return false;
}

bool AdsControl::isAvailableBannerAds()
{
#ifdef SDKBOX_ENABLED
	return sdkbox::PluginAdMob::isAvailable(kHomeBanner);
#endif
	return false;
}

