#pragma once
#include "2d/CCLayer.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
#include "Models/Tiles/Tile.h"

//#include "base/allocator/CCAllocatorStrategyPool.h"
class AnimationShowObject : public cocos2d::Layer
{
public:
	AnimationShowObject();
	~AnimationShowObject();

	virtual void initWithCSB(std::string csbFileName);

	virtual void reuse(const std::function<void()> callback);
	void recycle();

	void playAnimation();

	TileColors tileColor = TileColors::any;
	std::map<std::string, std::string>* customData = nullptr;
	std::string strData = "";

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

protected:
	Node* rootNode;
	cocostudio::timeline::ActionTimeline* action;
	std::string csbFileName;
};

class SpriteShowObject: public Sprite
{
public:
	SpriteShowObject(){};
	~SpriteShowObject(){};

	CREATE_FUNC(SpriteShowObject);

	void initWithTextureName(std::string textureName);
	void recycle() { removeFromParent(); }
};

class LabelShowObject : public Label
{
public:
	LabelShowObject() {};
	~LabelShowObject() {};

	CREATE_FUNC(LabelShowObject);

	void recycle() { removeFromParent(); }
};

class ParticleShowObject: public AnimationShowObject
{
public:
	void initWithCSB(std::string csbFileName) override;
	void reuse(const std::function<void()> callback) override;
	std::function<void()> recycleCallback;
	void scheduleToRecycle(float);
	ParticleSystem* colorParticle = nullptr;
	ParticleSystem* normalParticle = nullptr;
	float originSpeed;
};