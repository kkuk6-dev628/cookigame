#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Controllers/LevelController.h"
#include "Controllers/GameController.h"

class StartScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void initControllers();
	void releaseControllers();

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onEnterTransitionDidFinish();

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);

private:
	GameController* gameController = nullptr;
};

#endif // __HELLOWORLD_SCENE_H__
