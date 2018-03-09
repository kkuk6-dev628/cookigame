#include "Match.h"


bool Match::equal(Match* other)
{
	if(matchType != other->matchType)
	{
		return false;
	}
	for(auto cell : *vMatchedCells)
	{
		if (cell != other->refCell) 
		{
			if (std::find(other->vMatchedCells->begin(), other->vMatchedCells->end(), cell) == other->vMatchedCells->end())
			{
				return false;
			}
		}
	}
	for (auto cell : *hMatchedCells)
	{
		if (cell != other->refCell)
		{
			if (std::find(other->hMatchedCells->begin(), other->hMatchedCells->end(), cell) == other->hMatchedCells->end())
			{
				return false;
			}
		}
	}
	for (auto cell : *sMatchedCells)
	{
		if (cell != other->refCell)
		{
			if (std::find(other->sMatchedCells->begin(), other->sMatchedCells->end(), cell) == other->sMatchedCells->end())
			{
				return false;
			}
		}
	}
	return true;
}

void Match::crushAllCells() const
{
	crushCell(refCell);
	for (auto& cell : *hMatchedCells) crushCell(cell);
	for (auto& cell : *vMatchedCells) crushCell(cell);
	for (auto& cell : *sMatchedCells) crushCell(cell);
}

void Match::crushMatchedCells() const
{
	crushCell(refCell);
	if (hMatchedCells->size() > 2)
	{
		for (auto& cell : *hMatchedCells) crushCell(cell);
	}
	if (vMatchedCells->size() > 2)
	{
		for (auto& cell : *vMatchedCells) crushCell(cell);
	}
	if (sMatchedCells->size() > 3)
	{
		for (auto& cell : *sMatchedCells) crushCell(cell);
	}
}

void Match::crushCell(Cell* cell) const
{
	if (cell == nullptr || cell->isEmpty || cell->getSourceTile() == nullptr)
	{
		return;
	}
	poolController->recycleCookieTile(cell->getSourceTile());
	cell->clear();
}

bool Match::checkBonusCreated(MovingTileTypes bonusType)
{
	for (auto& cell : *hMatchedCells)
	{
		auto tile = cell->getSourceTile();

		if(tile != nullptr)
		{
			if(tile->getType() == bonusType._to_string())
			{
				return true;
			}
		}
	}
	for (auto& cell : *vMatchedCells)
	{
		auto tile = cell->getSourceTile();

		if (tile != nullptr)
		{
			if (tile->getType() == bonusType._to_string())
			{
				return true;
			}
		}
	}
	for (auto& cell : *sMatchedCells)
	{
		auto tile = cell->getSourceTile();

		if (tile != nullptr)
		{
			if (tile->getType() == bonusType._to_string())
			{
				return true;
			}
		}
	}
	return false;
}


MovingTileTypes Match::getAvailableBonusType()
{
	MovingTileTypes bonusType = MovingTileTypes::LayeredMatchObject;
	if (hMatchedCells->size() > 2)
	{
		if (hMatchedCells->size() > 4)
		{
			bonusType = MovingTileTypes::RainbowObject;
		}
		else if (vMatchedCells->size() > 2)
		{
			if ((left > 0 && right) || (up > 0 && down > 0))
			{
				bonusType = MovingTileTypes::XBreakerObject;
			}
			else
			{
				bonusType = MovingTileTypes::BombBreakerObject;
			}
		}
		else if (hMatchedCells->size() == 4)
		{
			bonusType = MovingTileTypes::ColumnBreakerObject;
		}
	}
	else if (vMatchedCells->size() > 2)
	{
		if (vMatchedCells->size() > 4)
		{
			bonusType = MovingTileTypes::RainbowObject;
		}
		else if (vMatchedCells->size() == 4)
		{
			//cocos2d::log("%d, %d, %d", match->hMatchedCells->size(), match->vMatchedCells->size(), match->sMatchedCells->size());
			bonusType = MovingTileTypes::RowBreakerObject;
		}
	}
	if (sMatchedCells->size() > 2)
	{
		if (bonusType == +MovingTileTypes::LayeredMatchObject)
		{
			//cocos2d::log("%d, %d, %d", match->hMatchedCells->size(), match->vMatchedCells->size(), match->sMatchedCells->size());
			bonusType = MovingTileTypes::SeekerObject;
		}
	}

	return bonusType;
}


Match::Match()
{
	hMatchedCells = new CellsList();
	vMatchedCells = new CellsList();
	sMatchedCells = new CellsList();
	poolController = PoolController::getInstance();
}


Match::~Match()
{
	CC_SAFE_DELETE(hMatchedCells);
	CC_SAFE_DELETE(vMatchedCells);
	CC_SAFE_DELETE(sMatchedCells);
}

Match* Match::create()
{
	auto match = new Match();
	match->autorelease();
	return match;
}
