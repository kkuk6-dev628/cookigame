#include "AnimationShowObject.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
#include "Controllers/PoolController.h"

//cocos2d::allocator::AllocatorStrategyPool<AnimationShowObject> AnimationShowObject::allocator( "AnimationShowObjectAllocator" );

AnimationShowObject::AnimationShowObject()
{
	rootNode = nullptr;
	action = nullptr;
	csbFileName = "";
}


AnimationShowObject::~AnimationShowObject()
{
}

void AnimationShowObject::reuse(const std::function<void()> callback)
{
	if(action == nullptr)
	{
		return;
	}
	action->gotoFrameAndPlay(0, false);
	action->setTimeSpeed(1.5);
	action->setLastFrameCallFunc(callback);
	rootNode->runAction(action);
}

void AnimationShowObject::recycle()
{
	removeFromParent();
	stopAllActions();
}


void AnimationShowObject::initWithCSB(std::string csbFileName)
{
	this->csbFileName = csbFileName;
	rootNode = cocos2d::CSLoader::createNode(csbFileName);
	rootNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	addChild(rootNode);

	action = cocos2d::CSLoader::createTimeline(csbFileName);
	action->retain();
}
