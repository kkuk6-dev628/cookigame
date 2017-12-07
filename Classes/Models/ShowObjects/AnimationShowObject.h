#pragma once
#include "2d/CCLayer.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"

//#include "base/allocator/CCAllocatorStrategyPool.h"
class AnimationShowObject : public cocos2d::Layer
{
public:
	AnimationShowObject();
	~AnimationShowObject();

	void initWithCSB(std::string csbFileName);

	void reuse(const std::function<void()> callback);
	void recycle();

	//static cocos2d::allocator::AllocatorStrategyPool<AnimationShowObject> allocator;
	//CC_USE_ALLOCATOR_POOL(AnimationShowObject, allocator)
	static AnimationShowObject * create()
	{
		AnimationShowObject * pRet = new  AnimationShowObject();
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = nullptr;
			return  nullptr;
		}
	}

private:
	Node* rootNode;
	cocostudio::timeline::ActionTimeline* action;
	std::string csbFileName;
};
