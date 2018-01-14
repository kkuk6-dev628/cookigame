#pragma once
#include "2d/CCNode.h"
#include "2d/CCAction.h"
#include "General/EnumTypes.h"


class FallPath;

class CKAction
{
public:
	cocos2d::Node* node;
	cocos2d::Action* action;

};

class ActionController
{
public:
	~ActionController();
	char getPendingActionCount();
	cocos2d::Action* createSeekerPendingAction(cocos2d::Node* node, cocos2d::Vec2 pos);
	cocos2d::Action* createSeekerLandingAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback);
	static ActionController* getInstance();

	void pushAction(CKAction ckAction, bool isPending) const;

	void runPendingActions() const;

	char getRunningActionCount();

	cocos2d::Action* createScaleBouncingAction(cocos2d::Node* node);
	cocos2d::Action* createDirectionalScaleAction(cocos2d::Node* node, AdjacentDirs dir);
	cocos2d::Action* createMoveThroughAction(FallPath* path, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createTileMoveAction(const cocos2d::Vec2& startPos, const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node) const;

	void endAction(cocos2d::Node* node) const;

	static float calcTileMovingTime(float distance);

private:
	ActionController();
	static ActionController* instance;

	std::map<cocos2d::Node*, CKAction>* pendingActions = nullptr;
	std::map<cocos2d::Node*, CKAction>* runningActions = nullptr;
};


