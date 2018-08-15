#include "FruitRollGroup.h"
#include "cocostudio/ActionTimeline/CSLoader.h"


FruitRollGroup::FruitRollGroup()
{
	fruitObjects = new std::vector<FruitRollObject*>;
	tailAnimation = CSLoader::createNode("res/FruitRollAnimation.csb");
	normalAction = CSLoader::createTimeline("res/FruitRollAnimation.csb");
	//normalAction->play("animation0", true);
	//action->setLastFrameCallFunc([=]() {
	//	action->clearLastFrameCallFunc();
	//	action->play("animation1", true);
	//});
	//tailAnimation->runAction(normalAction);
	normalAction->retain();
	tailAnimation->setContentSize(Size(CellSize, CellSize));
	tailAnimation->retain();
}


FruitRollGroup::~FruitRollGroup()
{
	CC_SAFE_DELETE(fruitObjects);
}

Cell* FruitRollGroup::rolling()
{
	if (rolled) return nullptr;

	if(fruitObjects->size() == 1)
	{
		
	}
	auto tail = fruitObjects->back();
	auto removeCell = tail->getCell();
	tail->removeFromParent();
	fruitObjects->pop_back();
	setTailAnimation(true);
	rolled = true;
	return removeCell;
}

void FruitRollGroup::setTailAnimation(bool showRollingAction)
{
	if (tailAnimation == nullptr || fruitObjects->size() == 0) return;
	auto tailFruit = fruitObjects->back();

	if(showRollingAction)
	{
		normalAction->play("animation1", true);
		if(tailFruit->isHead)
		{
			normalAction->setLastFrameCallFunc([=]() {
				tailFruit->setOneFruitTexture();
				tailAnimation->setVisible(false);
			});
		}
		else
		{
			normalAction->setLastFrameCallFunc([=]() {
				normalAction->clearLastFrameCallFunc();
				normalAction->play("animation0", true);
			});
		}
	}
	else
	{
		normalAction->play("animation0", true);
	}
	tailAnimation->removeFromParent();
	tailFruit->addChild(tailAnimation);
	tailAnimation->runAction(normalAction);
	//tailAnimation->setAnchorPoint(Vec2(0.5, 0.5));
	tailAnimation->setScale(0.6);
	tailFruit->setTextureRotation(tailAnimation);
	tailFruit->hideTexture();
}

CellsList* FruitRollGroup::getAllFruitCells()
{
	auto ret = new CellsList;
	for (auto fruit : *fruitObjects)
	{
		ret->push_back(fruit->getCell());
	}
	return ret;
}

char FruitRollGroup::getRmainderCount(FruitRollObject* fromObject)
{
	auto pos = std::find(fruitObjects->begin(), fruitObjects->end(), fromObject);
	if(pos == fruitObjects->end())
	{
		return 0;
	}

	return fruitObjects->size() - (pos - fruitObjects->begin());
}
