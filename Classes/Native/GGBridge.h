#ifndef  _APP_GGBRIDGE_H_
#define  _APP_GGBRIDGE_H_
#include "cocos2d.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
#include "platform/android/jni/JniHelper.h"  
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"  
#endif  


USING_NS_CC;
class GGBridge {
public:
	
	static bool hasInterstitialAd();
	static void showInterstitialAd();
	
	static void showBanner();
	static void hideBanner();

	static void rateApp();


	static void exitCP();
	static void showPopWindow();

	static const char* getPackageName();

	static void delayShowInterstitialAd(const float delay);
	static void delayShowInterstitialAd(const float delay,const int adpos);
	static void sendUMEvent(int levelId, int state);
	static void restartApp();
	static bool isPackageNameMatch();
	static bool hasRewardVideoAds();
	static void showRewardVideoAds();

	static const char* getVersion();
};
#endif


