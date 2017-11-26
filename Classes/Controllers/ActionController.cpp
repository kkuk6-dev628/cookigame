#include "ActionController.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include <array>
#include "General/EnumTypes.h"
#include "General/Constants.h"
#include "Models/BoardModels/Cell.h"

USING_NS_CC;

ActionController* ActionController::instance = nullptr;

ActionController::ActionController()
{
	runningActions = new std::map<Node*, CKAction>();
	pendingActions = new std::map<Node*, CKAction>();
}


ActionController::~ActionController()
{
	CC_SAFE_DELETE(runningActions);
	CC_SAFE_DELETE(pendingActions);
}

ActionController* ActionController::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ActionController();
	}
	return instance;
}

void ActionController::pushAction(CKAction ckAction, const bool isPending) const
{
	if (runningActions->size() > 0)
	{
		auto itr = runningActions->find(ckAction.node);
		if (itr != runningActions->end())
		{
			if (isPending)
			{
				(*pendingActions)[ckAction.node] = ckAction;
				return;
			}
			else
			{
				itr->first->stopAllActions();
				runningActions->erase(itr);
			}
		}
	}

	ckAction.node->runAction(ckAction.action);
	(*runningActions)[ckAction.node] = ckAction;
}

void ActionController::runPendingActions() const
{
	std::list<std::map<Node*, CKAction>::iterator> deleteItr;
	for (auto itr = pendingActions->begin(); itr != pendingActions->end(); ++itr)
	{
		if (runningActions->size() == 0 || runningActions->find(itr->first) == runningActions->end())
		{
			itr->first->runAction(itr->second.action);
			(*runningActions)[itr->first] = itr->second;
			deleteItr.push_back(itr);
		}
	}

	for (const auto litr : deleteItr)
	{
		pendingActions->erase(litr);
	}
}

Action* ActionController::createScaleBouncingAction(Node* node)
{
	Sequence* seq = Sequence::create(
		ScaleTo::create(0.2, 0.5),
		ScaleTo::create(0.2, 1.2),
		ScaleTo::create(0.2, 0.8), 
		ScaleTo::create(0.2, 1.1),
		ScaleTo::create(0.2, 1.0),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

Action* ActionController::createDirectionalScaleAction(Node* node, const AdjacentDirs dir)
{
	Sequence* seq = nullptr;
	switch (dir)
	{
	case AdjacentDirs::E:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.15, 0.5, 1),
				MoveTo::create(0.15, Vec2(CellSize * 3 / 4, CellSize / 2)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15, 1, 1),
				MoveTo::create(0.15, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::W:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.15, 0.5, 1),
				MoveTo::create(0.15, Vec2(CellSize / 4, CellSize / 2)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15, 1, 1),
				MoveTo::create(0.15, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::N:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.15, 1, 0.5),
				MoveTo::create(0.15, Vec2(CellSize / 2, CellSize * 3 / 4)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15, 1, 1),
				MoveTo::create(0.15, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::S:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.15, 1, 0.5),
				MoveTo::create(0.15, Vec2(CellSize / 2, CellSize / 4)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15, 1, 1),
				MoveTo::create(0.15, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	}
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createTileMoveAction(const cocos2d::Vec2& startPos, const cocos2d::Vec2& targetPos, std::function<void()> callback) const
{
	auto tileMovingTime = calcTileMovingTime(startPos.distance(targetPos));
	Sequence* seq = Sequence::create(
		MoveTo::create(0.2, targetPos),
		CallFunc::create(callback),
		nullptr);
	seq->retain();
	return seq;
}

void ActionController::endAction(cocos2d::Node* node) const
{
	if (runningActions->size() > 0)
	{
		auto itr = runningActions->find(node);
		if (itr != runningActions->end())
		{
			runningActions->erase(itr);
		}
	}

}

float ActionController::calcTileMovingTime(const float distance)
{
	return 0.2 + TileMovingTime * (distance / CellSize);
}
