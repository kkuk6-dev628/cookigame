#include "ActionController.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include <array>
#include "General/EnumTypes.h"
#include "General/Constants.h"
#include "Models/BoardModels/Cell.h"
#include "Models/BoardModels/FallPath.h"
#include "General/Utils.h"
#include "Ext/Math.h"

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
	if (runningActions != nullptr && runningActions->size() > 0)
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

	if(ckAction.delayCount > 0)
	{
		(*pendingActions)[ckAction.node] = ckAction;
	}
	else
	{
		ckAction.node->runAction(ckAction.action);
		(*runningActions)[ckAction.node] = ckAction;
	}
}

void ActionController::runPendingActions() const
{
	std::list<std::map<Node*, CKAction>::iterator> deleteItr;
	for (auto itr = pendingActions->begin(); itr != pendingActions->end(); ++itr)
	{
		if (runningActions->size() == 0 || runningActions->find(itr->first) == runningActions->end())
		{
			if(itr->second.delayCount > 0)
			{
				itr->second.delayCount--;
			}
			else
			{
				itr->first->runAction(itr->second.action);
				(*runningActions)[itr->first] = itr->second;
				deleteItr.push_back(itr);
			}
		}
	}

	for (const auto litr : deleteItr)
	{
		pendingActions->erase(litr);
	}
}

char ActionController::getRunningActionCount()
{
	if (runningActions == nullptr) return 0;

	return runningActions->size();
}

char ActionController::getPendingActionCount()
{
	if (pendingActions == nullptr) return 0;
	return pendingActions->size();
}

Action* ActionController::createSeekerPendingAction(Node* node, Vec2 pos)
{
	Sequence* seq = Sequence::create(
		Spawn::create(
			MoveTo::create(0.5f, pos),
			ScaleTo::create(0.5f, 1.5f),
			nullptr
			),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createSeekerLandingAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback)
{
	Sequence* seq = Sequence::create(
		Spawn::create(
			JumpTo::create(0.6f, pos, 2 * CellSize, 1),
			ScaleTo::create(0.6f, 1.0f),
			nullptr
		),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createShuffleMoveAction(cocos2d::Vec2 boardCenter, const cocos2d::Vec2& targetPos, std::function<void()> callback, Node* node)
{
	Sequence* seq = Sequence::create(
		JumpTo::create(0.4f, boardCenter, 2 * CellSize, 1),
		Hide::create(),
		DelayTime::create(0.8f),
		Show::create(),
		JumpTo::create(0.4f, targetPos, 2*CellSize, 1),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}


Action* ActionController::createScaleBouncingAction(Node* node)
{
	Sequence* seq = Sequence::create(
		ScaleTo::create(0.2f, 0.5f),
		ScaleTo::create(0.2f, 1.2f),
		ScaleTo::create(0.2f, 0.8f), 
		ScaleTo::create(0.2f, 1.1f),
		ScaleTo::create(0.2f, 1.0f),
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
				ScaleTo::create(0.15f, 0.5f, 1),
				MoveTo::create(0.15f, Vec2(CellSize * 3 / 4, CellSize / 2)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15f, 1, 1),
				MoveTo::create(0.15f, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::W:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.15f, 0.5f, 1),
				MoveTo::create(0.15f, Vec2(CellSize / 4, CellSize / 2)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15f, 1, 1),
				MoveTo::create(0.15f, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::N:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.15f, 1, 0.5f),
				MoveTo::create(0.15f, Vec2(CellSize / 2, CellSize * 3 / 4)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15f, 1, 1),
				MoveTo::create(0.15f, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::S:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.15f, 1, 0.5f),
				MoveTo::create(0.15f, Vec2(CellSize / 2, CellSize / 4)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.15f, 1, 1),
				MoveTo::create(0.15f, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	}
	seq->retain();
	return seq;
}

Action* ActionController::createMoveThroughAction(FallPath* path, std::function<void()> callback, Node* node)
{
	auto pathPos = node->getPosition();
	auto actions = Vector<FiniteTimeAction*>();
	actions.pushBack(DelayTime::create(0.05f));
	for (auto cell : path->fallPath)
	{
		if (cell->containsPortalOut())
		{
			actions.pushBack(MoveBy::create(calcTileMovingTime(CellSize), Vec2(0, -CellSize)));
			actions.pushBack(Hide::create());
			actions.pushBack(Place::create(cell->boardPos + Vec2(0, CellSize)));
			actions.pushBack(Show::create());
			actions.pushBack(MoveTo::create(calcTileMovingTime(CellSize), cell->boardPos));
		}
		else
		{
			auto boardPos = Utils::Grid2BoardPos(cell->gridPos);
			auto distance = pathPos.distance(cell->boardPos);
			if (distance < 50)
			{
				continue;
			}
			auto tileMovingTime = calcTileMovingTime(pathPos.distance(cell->boardPos));
			actions.pushBack(MoveTo::create(tileMovingTime, cell->boardPos));
		}
		pathPos = cell->boardPos;
	}
	auto lastPos = path->endCell->getBoardPos();
	auto t = calcTileMovingTime(pathPos.distance(lastPos));
	actions.pushBack(MoveTo::create(t, lastPos));

	actions.pushBack(MoveBy::create(0.04f, Vec2(0, 8)));
	actions.pushBack(MoveBy::create(0.06f, Vec2(0, 3)));
	actions.pushBack(MoveBy::create(0.06f, Vec2(0, -3)));
	actions.pushBack(MoveBy::create(0.04f, Vec2(0, -8)));

	actions.pushBack(CallFunc::create(callback)); 
	actions.pushBack(CallFunc::create([this, node]() { this->endAction(node); }));
	auto seq = Sequence::create(actions);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createTileMoveAction(const cocos2d::Vec2& startPos, const cocos2d::Vec2& targetPos, std::function<void()> callback, Node* node) const
{
	auto tileMovingTime = 0.15;
	Sequence* seq = Sequence::create(
		MoveTo::create(tileMovingTime, targetPos),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
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
	return 0.15 + TileMovingTime * (std::pow(distance / CellSize, 0.7f) - 1);
}
