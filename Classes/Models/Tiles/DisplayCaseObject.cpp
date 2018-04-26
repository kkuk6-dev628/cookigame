#include "DisplayCaseObject.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "General/Utils.h"
#include "Controllers/SpawnController.h"
#include "Models/BoardModels/Cell.h"

DisplayCaseObject::~DisplayCaseObject()
{
}

bool DisplayCaseObject::crush(bool showEffect)
{
	return false;
}

void DisplayCaseObject::resetNumberNode()
{
	for (char i = 1; i <= 5; i++)
	{
		auto name = StringUtils::format("%d", i);
		auto numberNode = rootNode->getChildByName(name);
		if (numberNode != nullptr)
		{
			if(i == power - 1)
			{
				numberNode->setVisible(true);
			}
			else
			{
				numberNode->setVisible(false);
			}
		}
	}
}

void DisplayCaseObject::initTexture()
{
	auto csbFileName = StringUtils::format("res/TailGlass.csb", type.c_str());
	rootNode = cocos2d::CSLoader::createNode(csbFileName);
	rootNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	rootNode->setPosition(CellSize / 2, CellSize / 2);
	addChild(rootNode);

	resetNumberNode();
}

bool DisplayCaseObject::refill()
{
	power--;
	if (power > 0)
	{
		resetNumberNode();

		auto spawnController = SpawnController::getInstance();
		spawnController->countSpawnedTiles();
		auto spawnColor = spawnController->getSpawnColor();
		std::string spawnType = ((MovingTileTypes)(MovingTileTypes::LayeredMatchObject))._to_string();

		auto spawnTable = spawnController->getSpawnTable(spawnTableName, totalSpawnedCount, pCell->inWater);
		if (spawnTable != nullptr)
		{
			spawnType = *spawnTable->Type;
		}
		auto spawnTile = static_cast<MovingTile*>(poolController->getCookieTile(spawnType));

		auto spawnedPos = Utils::Grid2BoardPos(gridPos);

		spawnTile->initWithGrid(gridPos.Col, gridPos.Row);
		spawnTile->setPosition(spawnedPos);
		spawnTile->initWithType(spawnType, spawnColor);
		pCell->setSourceTile(spawnTile);
		totalSpawnedCount++;
		return false;
	}

	rootNode->setVisible(false);
	return true;
}

void DisplayCaseObject::showCrushEffect()
{
	
}
