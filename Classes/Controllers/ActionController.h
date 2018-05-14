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
	cocos2d::Action* createHiderSegmentShakeAction(Node* node, bool isHorizental);
	static ActionController* getInstance();

	void pushAction(CKAction ckAction, bool isPending) const;

	void runPendingActions() const;

	char getRunningActionCount();

	cocos2d::Action* createSeekerPendingAction(cocos2d::Node* node, cocos2d::Vec2 pos);
	cocos2d::Action* createSeekerAndBonusPendingAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback);
	cocos2d::Action* createSeekerLandingAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback);
	cocos2d::Action* createShuffleMoveAction(cocos2d::Vec2 boardCenter, const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createJumpAction(cocos2d::Node* node, const cocos2d::Vec2& pos, float jumpHeight, std::function<void()> callback);
	cocos2d::Action* createMoveNumAction(cocos2d::Node* node, const cocos2d::Vec2& pos, float delayTime, float jumpTime, std::function<void()> callback);
	//cocos2d::Action* createPowerCollectAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback);
	cocos2d::Action* createFrontCrushAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> crushCallback, std::function<void()> lastCallback, char crushCount);

	cocos2d::Action* createScaleBouncingAction(std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createDirectionalScaleAction(cocos2d::Node* node, AdjacentDirs dir);
	cocos2d::Action* createMoveThroughAction(FallPath* path, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createTileMoveAction(const cocos2d::Vec2& startPos, const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node) const;
	cocos2d::Action* createPathFollowerMoveAction(const CellsList* path, std::function<void()> callback, cocos2d::Node* node) const;
	cocos2d::Action* createHiderSegmentMoveAction(CellsList* path, std::function<void()> callback, cocos2d::Node* node);

	cocos2d::Action* creatLightCircleShowAction(std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createHopplerMoveAction(CellsList* path, std::function<void()> callback, cocos2d::Node* node);

	cocos2d::Action* createPopLineAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createPopCollectionAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createHiderCollectionAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node);
	cocos2d::Action* createPopBombAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node);

	cocos2d::Action* createPieceSwappingAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node);

	Action* createScoreShowAction(std::function<void()> callback, cocos2d::Node* node);

	void endAction(cocos2d::Node* node) const;

	static float calcTileMovingTime(float distance);

private:
	ActionController();
	static ActionController* instance;

	std::map<cocos2d::Node*, CKAction>* pendingActions = nullptr;
	std::map<cocos2d::Node*, CKAction>* runningActions = nullptr;
};


