#include "MovingTile.h"
#include "Controllers/ActionController.h"
#include "General/Utils.h"


MovingTile::MovingTile()
{
}


MovingTile::~MovingTile()
{
}

void MovingTile::showScaleBouncingAction() const
{
	CKAction ckAction;
	auto actionController = ActionController::getInstance();
	ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	ckAction.action = actionController->createScaleBouncingAction(ckAction.node);
	actionController->pushAction(ckAction, false);
}

void MovingTile::showDirectionalScaleAction(const AdjacentDirs dir) const
{
	CKAction ckAction;
	auto actionController = ActionController::getInstance();
	ckAction.node = reinterpret_cast<Node*>(this->textureSprite);
	ckAction.action = actionController->createDirectionalScaleAction(ckAction.node, dir);
	actionController->pushAction(ckAction, false);
}

bool MovingTile::isMovable() const
{
	return true;
}

void MovingTile::showSwapAction(GridPos& gridPos, const std::function<void()> callback)
{
	const auto targetPos = Utils::Grid2BoardPos(gridPos);
	CKAction ckAction;
	const auto actionController = ActionController::getInstance();
	ckAction.node = reinterpret_cast<Node*>(this);
	ckAction.action = actionController->createTileMoveAction(getPosition(), targetPos, callback);
	actionController->pushAction(ckAction, false);
}

//void MovingTile::onEnterTransitionDidFinish()
//{
//	Node::onEnterTransitionDidFinish();
//
//	// touches
//	auto touchListener = EventListenerTouchOneByOne::create();
//	touchListener->onTouchBegan = CC_CALLBACK_2(MovingTile::onTouchBegan, this);
//	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
//
//	//start
//	scheduleUpdate();
//}
//
//bool MovingTile::onTouchBegan(Touch* touch, Event* unused_event)
//{
//	CKAction ckAction;
//	auto actionController = ActionController::getInstance();
//	ckAction.node = reinterpret_cast<Node*>(this);
//	ckAction.action = actionController->createScaleBouncingAction(ckAction.node);
//	actionController->pushAction(ckAction, false);
//	return true;
//}
