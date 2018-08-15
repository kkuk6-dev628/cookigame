#pragma once
#include <vector>
#include "FruitRollObject.h"

class Cell;

class FruitRollGroup
{
public:
	FruitRollGroup();
	~FruitRollGroup();

	std::vector<FruitRollObject*>* getFruitObjectsList() const { return fruitObjects; }

	void addFruitObject(FruitRollObject* fruitObject) const { fruitObjects->push_back(fruitObject); };

	Cell* rolling();

	void setTailAnimation(bool showRollingAction = false);
	void initRollFlag() { rolled = false; }

	CellsList* getAllFruitCells();

	char getRmainderCount(FruitRollObject* fromObject);

private:
	std::vector<FruitRollObject*>* fruitObjects = nullptr;
	Node* tailAnimation = nullptr;
	cocostudio::timeline::ActionTimeline* normalAction = nullptr;
	cocostudio::timeline::ActionTimeline* rollingAction = nullptr;
	bool rolled = false;
};

