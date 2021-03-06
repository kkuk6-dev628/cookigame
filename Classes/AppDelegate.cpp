#include "AppDelegate.h"
#include "Scenes/StartScene.h"
#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(SCREEN_X, SCREEN_Y);
static cocos2d::Size smallResolutionSize = cocos2d::Size(360, 640);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(720, 1440);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1440, 2560);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
	//_CrtDumpMemoryLeaks();
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

#ifdef SDKBOX_ENABLED
	sdkbox::PluginAdMob::init();
#endif

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        //glview = GLViewImpl::createWithRect("Cookie", cocos2d::Rect(0, 0, 480, 860)); // 1.9 : 1
		glview = GLViewImpl::createWithRect("Cookie", cocos2d::Rect(0, 0, 420, 860)); // 2.05 : 1
#else
        glview = GLViewImpl::create("Cookie");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
	FileUtils::getInstance()->addSearchPath("res");

    // Set the design resolution
    //glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// design resolution
	float designAR(designResolutionSize.width / designResolutionSize.height);

	CCLOG("design res width: %4.4f; height: %4.4f; ar: %4.4f;\n", designResolutionSize.width, designResolutionSize.height, designAR);


	// device resolution
	Size deviceSize(Director::getInstance()->getOpenGLView()->getFrameSize());
	float deviceAR(deviceSize.width / deviceSize.height);

	CCLOG("device res width: %4.4f; height: %4.4f; ar: %4.4f;\n", deviceSize.width, deviceSize.height, deviceAR);

	// final design size, enlarged if aspect ratio is different than 3:2
	Size finalDesignSize(designResolutionSize);

	// wide screen
	if (deviceAR > designAR)
	{
		finalDesignSize.width = finalDesignSize.width * (deviceAR / designAR);
	}
	// narrow screen
	else if (deviceAR < designAR)
	{
		finalDesignSize.height = finalDesignSize.height * (designAR / deviceAR);
	}

	float finalDesignAR(finalDesignSize.width / finalDesignSize.height);
	DesignResolutionSize = finalDesignSize;
	glview->setDesignResolutionSize(finalDesignSize.width, finalDesignSize.height, ResolutionPolicy::EXACT_FIT);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto frameSize = glview->getFrameSize();

	//CellSize = (frameSize.width - 9) / 9;

    // if the frame's height is larger than the height of medium size.
    //if (frameSize.height > mediumResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is larger than the height of small size.
    //else if (frameSize.height > smallResolutionSize.height)
    //{        
    //    director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    //}
    //// if the frame's height is smaller than the height of medium size.
    //else
    //{        
    //    director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    //}

    register_all_packages();

    // create a scene. it's an autorelease object
	auto scene = StartScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
