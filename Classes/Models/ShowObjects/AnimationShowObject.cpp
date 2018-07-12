#include "AnimationShowObject.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
#include "Controllers/PoolController.h"
#include "spine/SkeletonAnimation.h"

//cocos2d::allocator::AllocatorStrategyPool<AnimationShowObject> AnimationShowObject::allocator( "AnimationShowObjectAllocator" );

AnimationShowObject::AnimationShowObject()
{
	rootNode = nullptr;
	action = nullptr;
	csbFileName = "";
	customData = new std::map<std::string, std::string>;
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
	if(callback == nullptr)
	{
		action->gotoFrameAndPlay(0, true);
	}
	else
	{
		action->gotoFrameAndPlay(0, false);
		action->setLastFrameCallFunc(callback);
	}
	action->setTimeSpeed(1.5);
	rootNode->runAction(action);
}

void AnimationShowObject::playAnimation()
{
	if (action == nullptr)
	{
		return;
	}
	action->gotoFrameAndPlay(0, false);
	action->setLastFrameCallFunc([=]{
		this->setVisible(false);
	});
	rootNode->stopAllActions();
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


void SpriteShowObject::initWithTextureName(std::string textureName)
{
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);
	setSpriteFrame(spriteFrame);
	//setContentSize(Size(CellSize, CellSize));
	setAnchorPoint(Vec2(0.5, 0.5));
}

void ParticleShowObject::initWithCSB(std::string csbFileName)
{
	AnimationShowObject::initWithCSB(csbFileName);
	colorParticle = static_cast<ParticleSystem*>(rootNode->getChildren().at(0));
}

void ParticleShowObject::reuse(const std::function<void()> callback)
{
	if (colorParticle != nullptr)
	{
		//colorParticle->setSpeed(colorParticle->getSpeed() * 2);
		colorParticle->resetSystem();
	}

	recycleCallback = callback;
	scheduleOnce(schedule_selector(ParticleShowObject::scheduleToRecycle), 0.4);
}

void ParticleShowObject::scheduleToRecycle(float dt)
{
	if(recycleCallback != nullptr)
	{
		recycleCallback();
	}
}

void SpineShowObject::initWithCSB(std::string csbFileName)
{
}

void SpineShowObject::initWithSpine(std::string jsonFileName, std::string atlasFileName, std::string animationName)
{
	spineAnim = spine::SkeletonAnimation::createWithJsonFile(jsonFileName, atlasFileName);
	spineAnim->setAnimation(0, animationName, true);
	spineAnim->setAnchorPoint(Vec2(0.5f, 0.5f));
	spineAnim->setPosition(Vec2(0, 0));
	addChild(spineAnim);
	setAnchorPoint(Vec2(0.5f, 0.5f));
}

void SpineShowObject::reuse(const std::function<void()> callback)
{
	spineAnim->setCompleteListener([=](int trackIndex, int loopCount) {
		callback();
	});
}
