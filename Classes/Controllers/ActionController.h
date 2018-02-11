#pragma once
#include "2d/CCNode.h"
#include "2d/CCAction.h"
#include "General/EnumTypes.h"
#include "Models/BoardModels/Cell.h"


class FallPath;

class CKAction
{
public:
	cocos2d::Node* node;
	cocos2d::Action* action;
	char delayCount = 0;
};

class ActionController
{
public:
	~ActionController();
	char getPendingActionCount();
	static ActionController* getInstance();

	void pushAction(CKAction ckAction, bool isPending) const;

	void runPendingActions() const;

	char getRunningActionCount();

	cocos2d::Action* createSeekerPendingAction(cocos2d::Node* node, cocos2d::Vec2 pos);
	cocos2d::Action* createSeekerLandingAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback);
	cocos2d::Action* createShuffleMoveAction(cocos2d::Vec2 boardCenter, const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createJumpAction(cocos2d::Node* node, const cocos2d::Vec2& pos, float jumpHeight, std::function<void()> callback);
	//cocos2d::Action* createPowerCollectAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback);
	cocos2d::Action* createFrontCrushAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> crushCallback, std::function<void()> lastCallback, char crushCount);

	cocos2d::Action* createScaleBouncingAction(cocos2d::Node* node);
	cocos2d::Action* createDirectionalScaleAction(cocos2d::Node* node, AdjacentDirs dir);
	cocos2d::Action* createMoveThroughAction(FallPath* path, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createTileMoveAction(const cocos2d::Vec2& startPos, const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node) const;

	cocos2d::Action* creatLightCircleShowAction(std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createHopplerMoveAction(std::list<Cell*>* path, std::function<void()> callback, cocos2d::Node* node);

	void endAction(cocos2d::Node* node) const;

	static float calcTileMovingTime(float distance);

private:
	ActionController();
	static ActionController* instance;

	std::map<cocos2d::Node*, CKAction>* pendingActions = nullptr;
	std::map<cocos2d::Node*, CKAction>* runningActions = nullptr;
};


