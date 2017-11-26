#pragma once
#include "2d/CCNode.h"
#include "2d/CCAction.h"
#include "General/EnumTypes.h"


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
	static ActionController* getInstance();

	void pushAction(CKAction ckAction, bool isPending) const;

	void runPendingActions() const;

	cocos2d::Action* createScaleBouncingAction(cocos2d::Node* node);
	cocos2d::Action* createDirectionalScaleAction(cocos2d::Node* node, AdjacentDirs dir);
	cocos2d::Action* createTileMoveAction(const cocos2d::Vec2& startPos, const cocos2d::Vec2& targetPos, std::function<void()> callback) const;

	void endAction(cocos2d::Node* node) const;

	static float calcTileMovingTime(float distance);

private:
	ActionController();
	static ActionController* instance;

	std::map<cocos2d::Node*, CKAction>* pendingActions;
	std::map<cocos2d::Node*, CKAction>* runningActions;
};


