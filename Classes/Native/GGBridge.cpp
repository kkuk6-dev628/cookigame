#include "GGBridge.h"
#include "Layers/Dialogs.h"

const char* const ADS_CLASS_NAME = "org/cocos2dx/cpp/AdsController";
const char* const CP_CLASS_NAME = "org/cocos2dx/cpp/CPController";
const char* const APP_CLASS_NAME = "nz.popstar.pandapp.bubbleshooter.yx";
void callStaticVoidMethod(const char* className,const char* name)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
int callStaticIntMethod(const char* className, const char* name) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "()I")) {
		int r=t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		return r;
	}
	
#endif
	return 0;
}

bool callStaticBoolMethodBoolean(const char* className, const char* name)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "()Z")) {
		bool result = (t.env->CallStaticBooleanMethod(t.classID, t.methodID) == JNI_TRUE);
		t.env->DeleteLocalRef(t.classID);
		return result;
	}
	else {
		return false;
	}
#endif
	return false;
}

void callStaticVoidMethodWithString(const char* className, const char* name, const char* idName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "(Ljava/lang/String;)V")) {
		jstring jIdName = t.env->NewStringUTF(idName);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jIdName);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(jIdName);
	}
#endif
}

void callStaticVoidMethodWithStringAndInt(const char* className, const char* name, const char* idName, const int score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "(Ljava/lang/String;I)V")) {
		jstring jIdName = t.env->NewStringUTF(idName);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jIdName, (jint)score);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(jIdName);
	}
#endif
}

void callStaticVoidMethodWithInt(const char* className, const char* name, const int score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, (jint)score);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
void callStaticVoidMethodWithFloat(const char* className, const char* name, const float delay)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "(F)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, delay);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}
void callStaticVoidMethodWithFloatInt(const char* className, const char* name, const float delay,const int i)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "(FI)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, delay,i);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void callStaticVoidMethodWithIntAndInt(const char* className, const char* name, const int levelID, const int state)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, className, name, "(II)V")) {

		t.env->CallStaticVoidMethod(t.classID, t.methodID, (jint)levelID, (jint)state);
		t.env->DeleteLocalRef(t.classID);

	}
#endif
}
void GGBridge::sendUMEvent(const int levelId, const int state) {
	//callStaticVoidMethodWithIntAndInt("org/cocos2dx/cpp/AppActivity","sendLevelLog", levelId, state);
}
void GGBridge::restartApp() {
	//callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "restartApp");
}

bool GGBridge::hasInterstitialAd()
{	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	//return callStaticBoolMethodBoolean(ADS_CLASS_NAME, "hasInterstitial");
#endif
	return true;
}
bool GGBridge::hasRewardVideoAds()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	//return callStaticBoolMethodBoolean(ADS_CLASS_NAME, "hasRewardVideoAds");
#endif
	return true;
}
void GGBridge::showInterstitialAd()
{		
	//callStaticVoidMethod(ADS_CLASS_NAME, "showInterstitial");
}
void GGBridge::showRewardVideoAds()
{		
	//callStaticVoidMethod(ADS_CLASS_NAME, "showRewardVideoAds");
}
void GGBridge::delayShowInterstitialAd(const float delay)
{
	//callStaticVoidMethodWithFloat(ADS_CLASS_NAME, "delayShowInterstitial", delay);
}
void GGBridge::delayShowInterstitialAd(const float delay, const int adpos)
{
	//callStaticVoidMethodWithFloatInt(ADS_CLASS_NAME, "delayShowInterstitial",delay,adpos);
}
void GGBridge::showBanner()
{
	//callStaticVoidMethod(ADS_CLASS_NAME, "showBanner");
}

void GGBridge::hideBanner()
{
	//callStaticVoidMethod(ADS_CLASS_NAME, "hideBanner");
}

void GGBridge::rateApp()
{
	//callStaticVoidMethod(CP_CLASS_NAME, "rateApp");	
}


void GGBridge::exitCP()
{
	//callStaticVoidMethod(CP_CLASS_NAME,"exitAlert");
}

void GGBridge::showPopWindow()
{
	//callStaticVoidMethod(CP_CLASS_NAME,"showPopWindow");
}

const char * GGBridge::getPackageName()
{	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 	
	return getPackageNameJNI().c_str();
#endif
	return "";
}

bool GGBridge::isPackageNameMatch()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 	
	return strcmp(APP_CLASS_NAME, getPackageName())==0;
#else
	return true;
#endif	
}

const char *GGBridge::getVersion() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	return JniHelper::callStaticStringMethod(CP_CLASS_NAME, "ApkVersion").c_str();
#else
	
	return "1.1";
#endif
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
extern "C"
{
	void Java_org_cocos2dx_cpp_AdsController_RewardedVideoFinish(JNIEnv*  env, jobject thiz)
	{
		//CCNotificationCenter::sharedNotificationCenter()->postNotification(kMSG_BuyDimonds, (CCObject*)bSuccess);

		if (ShopDialog::getInstance())
			ShopDialog::getInstance()->rewardedVideoFinish();

	}
}
#endif
