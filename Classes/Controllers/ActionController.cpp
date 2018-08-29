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
	try
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

		if (ckAction.delayCount > 0)
		{
			(*pendingActions)[ckAction.node] = ckAction;
		}
		else
		{
			ckAction.node->runAction(ckAction.action);
			(*runningActions)[ckAction.node] = ckAction;
		}
	}
	catch (const std::exception&)
	{

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

cocos2d::Action* ActionController::createHiderSegmentShakeAction(Node* node, bool isHorizental)
{
	float x = isHorizental ? 1.3 : 1;
	float y = isHorizental ? 1 : 1.3;
	Sequence* seq = Sequence::create(
		Spawn::create(
			Sequence::create(
				ScaleTo::create(0.1, x, y),
				ScaleTo::create(0.1, 1, 1),
				ScaleTo::create(0.1, x, y),
				ScaleTo::create(0.1, 1, 1),
				ScaleTo::create(0.1, x, y),
				ScaleTo::create(0.1, 1, 1),
				ScaleTo::create(0.1, x, y),
				ScaleTo::create(0.1, 1, 1),
				nullptr
			),
			Sequence::create(
				RotateBy::create(0.1, 5),
				RotateBy::create(0.1, -10),
				RotateBy::create(0.1, 10),
				RotateBy::create(0.1, -10),
				RotateBy::create(0.1, 10),
				RotateBy::create(0.1, -10),
				RotateBy::create(0.1, 10),
				RotateBy::create(0.1, -5),
				nullptr
				),
			nullptr),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

Action* ActionController::createSeekerPendingAction(Node* node, Vec2 pos)
{
	Sequence* seq = Sequence::create(
		Spawn::create(
			MoveTo::create(0.3f, pos),
			ScaleTo::create(0.3f, 1.5f),
			nullptr
			),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createSeekerAndBonusPendingAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback)
{
	Sequence* seq = Sequence::create(
		Spawn::create(
			MoveTo::create(0.3f, pos),
			ScaleTo::create(0.3f, 0.8f),
			nullptr
		),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createSeekerLandingAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> callback)
{
	Sequence* seq = Sequence::create(
		Spawn::create(
			JumpTo::create(0.3f, pos, 2 * CellSize, 1),
			ScaleTo::create(0.3f, 1.0f),
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

cocos2d::Action* ActionController::createJumpAction(cocos2d::Node* node, const cocos2d::Vec2& pos, float jumpHeight, std::function<void()> callback)
{
	Sequence* seq = Sequence::create(
		Spawn::create(
			JumpTo::create(0.4f, pos, jumpHeight, 1),
			Sequence::create(
				ScaleTo::create(0.2f, 1.2f),
				ScaleTo::create(0.2f, 1.0f),
				nullptr),
			nullptr
		),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createMoveNumAction(cocos2d::Node* node, const cocos2d::Vec2& pos, float delayTime, float jumpTime, std::function<void()> callback)
{
	Sequence* seq = Sequence::create(
		DelayTime::create(delayTime),
		JumpTo::create(jumpTime, pos, CellSize, 1),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createFrontCrushAction(cocos2d::Node* node, const cocos2d::Vec2& pos, std::function<void()> crushCallback, std::function<void()> lastCallback, char crushCount)
{
	auto originPos = node->getPosition();
	auto actions = Vector<FiniteTimeAction*>();

	for(char i = 0; i < crushCount; i++)
	{
		actions.pushBack(JumpTo::create(0.4f, pos, 2 * CellSize, 1));
		actions.pushBack(CallFunc::create(crushCallback));
		actions.pushBack(JumpTo::create(0.4f, originPos, CellSize / 2, 1));
	}
	actions.pushBack(CallFunc::create(lastCallback));
	actions.pushBack(CallFunc::create([this, node]() { this->endAction(node); }));
	Sequence* seq = Sequence::create(actions);
	seq->retain();
	return seq;
}

Action* ActionController::createScaleBouncingAction(std::function<void()> callback, Node* node)
{
	Sequence* seq = Sequence::create(
		ScaleTo::create(0.05f, 0.8f),
		ScaleTo::create(0.05f, 1.1f),
		ScaleTo::create(0.05f, 1.0f), 
		CallFunc::create(callback),
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
				ScaleTo::create(0.1f, 0.5f, 1),
				MoveTo::create(0.1f, Vec2(CellSize * 3 / 4, CellSize / 2)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.1f, 1, 1),
				MoveTo::create(0.1f, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::W:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.1f, 0.5f, 1),
				MoveTo::create(0.1f, Vec2(CellSize / 4, CellSize / 2)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.1f, 1, 1),
				MoveTo::create(0.1f, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::N:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.1f, 1, 0.5f),
				MoveTo::create(0.1f, Vec2(CellSize / 2, CellSize * 3 / 4)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.1f, 1, 1),
				MoveTo::create(0.1f, Vec2(CellSize / 2, CellSize / 2)),
				nullptr
			),
			CallFunc::create([this, node]() { this->endAction(node); }),
			nullptr);
		break;
	case AdjacentDirs::S:
		seq = Sequence::create(
			Spawn::create(
				ScaleTo::create(0.1f, 1, 0.5f),
				MoveTo::create(0.1f, Vec2(CellSize / 2, CellSize / 4)),
				nullptr
			),
			Spawn::create(
				ScaleTo::create(0.1f, 1, 1),
				MoveTo::create(0.1f, Vec2(CellSize / 2, CellSize / 2)),
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
	auto actions = Vector<FiniteTimeAction*>();
	actions.pushBack(DelayTime::create(0.02f));
	auto pathPos = node->getPosition();
	if (path->startCell->containsPortalIn())
	{
		//auto firstPath = path->fallPath.size() > 0 ? path->fallPath.front() : path->endCell;
		char moveSign = path->startCell->inWater ? -1 : 1;
		actions.pushBack(MoveBy::create(calcTileMovingTime(CellSize / 2), Vec2(0, -moveSign*CellSize)));
		actions.pushBack(Hide::create());
		//actions.pushBack(Place::create(firstPath->boardPos + Vec2(0, CellSize)));
		//actions.pushBack(Show::create());
		//pathPos = firstPath->boardPos + Vec2(0, CellSize);
	}
	for (auto cell : path->fallPath)
	{
		char moveSign = cell->inWater ? -1 : 1;
		if (cell->containsPortalOut()) // && pathPos.y < cell->getBoardPos().y)
		{
			actions.pushBack(Place::create(cell->boardPos + Vec2(0, moveSign * CellSize)));
			actions.pushBack(Show::create());
			pathPos = cell->boardPos;
		}
		auto distance = pathPos.distance(cell->boardPos);
		if (distance < 50)
		{
			continue;
		}
		auto tileMovingTime = calcTileMovingTime(pathPos.distance(cell->boardPos));
		actions.pushBack(MoveTo::create(tileMovingTime, cell->boardPos));
		pathPos = cell->boardPos;
		if (cell->containsPortalIn()) 
		{
			actions.pushBack(MoveBy::create(calcTileMovingTime(CellSize / 2), Vec2(0,  - moveSign * CellSize)));
			actions.pushBack(Hide::create());
		}
	}

	if (path->endCell->containsPortalOut()) // && pathPos.y < path->endCell->getBoardPos().y)
	{
		char moveSign = path->endCell->inWater ? -1 : 1;
		pathPos = path->endCell->boardPos + Vec2(0, moveSign * CellSize);
		//actions.pushBack(MoveBy::create(calcTileMovingTime(CellSize / 2), Vec2(0, -CellSize)));
		//actions.pushBack(Hide::create());
		actions.pushBack(Place::create(pathPos));
		actions.pushBack(Show::create());
	}

	actions.pushBack(Show::create());

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
	auto tileMovingTime = 0.1;
	Sequence* seq = Sequence::create(
		MoveTo::create(tileMovingTime, targetPos),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createPathFollowerMoveAction(const CellsList* path, std::function<void()> callback, cocos2d::Node* node) const
{
	auto actions = Vector<FiniteTimeAction*>();
	auto pathPos = node->getPosition();
	for (auto cell : *path)
	{
		auto distance = pathPos.distance(cell->boardPos);
		auto movingTime = calcTileMovingTime(distance);
		actions.pushBack(MoveTo::create(movingTime, cell->getBoardPos()));
		pathPos = cell->getBoardPos();
	}
	actions.pushBack(CallFunc::create(callback));
	actions.pushBack(CallFunc::create([this, node]() { this->endAction(node); }));
	auto seq = Sequence::create(actions);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createHiderSegmentMoveAction(CellsList* path, std::function<void()> callback, cocos2d::Node* node)
{
	auto actions = Vector<FiniteTimeAction*>();
	actions.pushBack(DelayTime::create(0.05f));
	for (auto itr = path->rbegin(); itr != path->rend(); ++itr)
	{
		auto cell = *itr;
		actions.pushBack(MoveTo::create(0.1f, cell->boardPos));
	}

	actions.pushBack(CallFunc::create(callback));
	actions.pushBack(CallFunc::create([this, node]() { this->endAction(node); }));
	auto seq = Sequence::create(actions);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::creatLightCircleShowAction(std::function<void()> callback, cocos2d::Node* node)
{
	Sequence* seq = Sequence::create(
		FadeIn::create(0.05),
		DelayTime::create(0.1),
		FadeOut::create(0.05),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createTopplerMoveAction(CellsList* path, std::function<void()> callback, cocos2d::Node* node)
{
	auto actions = Vector<FiniteTimeAction*>();
	auto pathPos = node->getPosition();
	for(auto cell : *path)
	{
		auto distance = pathPos.distance(cell->boardPos);
		auto movingTime = calcTileMovingTime(distance);
		actions.pushBack(MoveTo::create(movingTime, cell->getBoardPos()));
		pathPos = cell->getBoardPos();
	}
	actions.pushBack(CallFunc::create(callback));
	actions.pushBack(CallFunc::create([this, node]() { this->endAction(node); }));
	auto seq = Sequence::create(actions);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createPopLineAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node)
{
	auto tileMovingTime = 0.3;
	Sequence* seq = Sequence::create(
		MoveTo::create(tileMovingTime, targetPos),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createPopCollectionAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node)
{
	auto tileMovingTime = 0.4;
	Sequence* seq = Sequence::create(
		ScaleTo::create(0.1, 1.2),
		ScaleTo::create(0.1, 0.8),
		ScaleTo::create(0.1, 1.1),
		ScaleTo::create(0.1, 1.0),
		MoveTo::create(tileMovingTime, targetPos),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createHiderCollectionAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node)
{
	Sequence* seq = Sequence::create(
		ScaleTo::create(0.1, 1.6),
		ScaleTo::create(0.1, 1.2),
		ScaleTo::create(0.1, 1.4),
		ScaleTo::create(0.1, 1.0),
		MoveTo::create(0.4, targetPos),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}


cocos2d::Action* ActionController::createPopBombAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node)
{
	auto tileMovingTime = 0.4;
	Sequence* seq = Sequence::create(
		ScaleTo::create(0.1, 1.2),
		ScaleTo::create(0.1, 0.8),
		ScaleTo::create(0.1, 1.1),
		ScaleTo::create(0.1, 1.0),
		MoveTo::create(tileMovingTime, targetPos),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

cocos2d::Action* ActionController::createPieceSwappingAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node)
{
	auto tileMovingTime = 0.2;
	Sequence* seq = Sequence::create(
		Spawn::create(
			ScaleTo::create(tileMovingTime, 0),
			RotateTo::create(tileMovingTime, 180),
			nullptr
		),
		Place::create(targetPos),
		Spawn::create(
			ScaleTo::create(tileMovingTime, 1),
			RotateTo::create(tileMovingTime, 360),
			nullptr
		),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

Action* ActionController::createSwapBoosterAction(const cocos2d::Vec2& targetPos, std::function<void()> callback, cocos2d::Node* node)
{
	Sequence* seq = Sequence::create(
		MoveTo::create(0.2, targetPos),
		CallFunc::create(callback),
		CallFunc::create([this, node]() { this->endAction(node); }),
		nullptr);
	seq->retain();
	return seq;
}

Action* ActionController::createScoreShowAction(std::function<void()> callback, cocos2d::Node* node)
{
	Sequence* seq = Sequence::create(
		Spawn::create(
			MoveBy::create(0.5, Vec2(0, 1.2 * CellSize)),
			FadeOut::create(0.5),
			nullptr
		),
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
	return 0.1 + TileMovingTime * (std::pow(distance / CellSize, 0.7f) - 1);
}
