#include "Utils.h"
#include "Constants.h"
#include "Models/BoardModels/Cell.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}

bool Utils::containsCell(std::list<Cell*>* cellsList, Cell* cell)
{
	if(cellsList == nullptr || cellsList->size() == 0 || cell == nullptr)
	{
		return false;
	}
	auto itr = std::find(cellsList->begin(), cellsList->end(), cell);
	return itr != cellsList->end();
}

GridSizeT Utils::StrToGridSize(std::string str, std::string delim)
{
	auto end = str.find(delim);
	assert(end != std::string::npos);
	auto strCol = str.substr(0, end);
	auto strRow = str.substr(end, str.length() - end - delim.length());
	GridSizeT gridSize;
	gridSize.Width = atoi(strCol.c_str());
	gridSize.Height = atoi(strRow.c_str());
	return gridSize;
}

GridPos Utils::StrToGridPos(std::string str, std::string delim)
{
	auto end = str.find(delim);
	assert(end != std::string::npos);
	auto strCol = str.substr(0, end);
	auto strRow = str.substr(end + delim.length(), str.length() - end - delim.length());
	GridPos gridPos;
	gridPos.Col = atoi(strCol.c_str());
	gridPos.Row = atoi(strRow.c_str());
	return gridPos;
}

cocos2d::Vec2 Utils::Grid2BoardPos(char col, char row, float boardWidth, float boardHeight)
{
	return cocos2d::Vec2(col * CellSize + CellSize / 2, row * CellSize + CellSize / 2);
}

cocos2d::Vec2 Utils::Grid2BoardPos(GridPos& gridPos)
{
	return cocos2d::Vec2(gridPos.Col * CellSize + CellSize / 2, gridPos.Row * CellSize + CellSize / 2);
}

GridPos Utils::Board2GridPos(const cocos2d::Vec2 boardPos)
{
	GridPos gridPos;
	gridPos.Col = floor(boardPos.x / CellSize);
	gridPos.Row = floor(boardPos.y / CellSize);
	return gridPos;
}

bool Utils::IsSameGrid(const GridPos gridA, const GridPos gridB)
{
	return gridA.Col == gridB.Col && gridA.Row == gridB.Row;
}

AdjacentDirs Utils::getDirection(GridPos& posA, GridPos& posB)
{
	if (posA.Col > posB.Col) 
	{
		if (posA.Row > posB.Row)
		{
			return AdjacentDirs::SW;
		}
		if (posA.Row < posB.Row)
		{
			return AdjacentDirs::NW;
		}
		return AdjacentDirs::W;
	}
	if (posA.Col < posB.Col) {
		if (posA.Row > posB.Row)
		{
			return AdjacentDirs::SE;
		}
		if (posA.Row < posB.Row)
		{
			return AdjacentDirs::NE;
		}
		return AdjacentDirs::E;
	}
	// posA.Col == posB.Col
	if (posA.Row > posB.Row)
	{
		return AdjacentDirs::S;
	}
	if (posA.Row < posB.Row)
	{
		return AdjacentDirs::N;
	}

	// posA.Col == posB.Col && posA.Row == posB.Row i.e. the same tiles
	return AdjacentDirs::NoDir;

}

AdjacentDirs Utils::getDirection(const cocos2d::Vec2 offset)
{
	if (abs(offset.y) >= abs(offset.x))
	{
		if (offset.y > 0) return AdjacentDirs::N;
		if (offset.y < 0) return AdjacentDirs::S;
	}
	else
	{
		if (offset.x > 0) return AdjacentDirs::E;
		if (offset.x < 0) return AdjacentDirs::W;
	}
	return AdjacentDirs::NoDir;
}

GridPos Utils::getAdjacentPos(GridPos& pos, const AdjacentDirs dir)
{
	GridPos ret;
	ret.Col = pos.Col; ret.Row = pos.Row;
	switch (dir)
	{
	case AdjacentDirs::E:
		ret.Col++;
		break;
	case AdjacentDirs::W:
		ret.Col--;
		break;
	case AdjacentDirs::S:
		ret.Row--;
		break;
	case AdjacentDirs::N:
		ret.Row++;
		break;
	case AdjacentDirs::NE:
		ret.Col++; ret.Row++;
		break;
	case AdjacentDirs::NW:
		ret.Col--; ret.Row++;
		break;
	case AdjacentDirs::SE:
		ret.Col++; ret.Row--;
		break;
	case AdjacentDirs::SW:
		ret.Col--; ret.Row--;
		break;
	}
	return ret;
}

AdjacentDirs Utils::inverseDir(AdjacentDirs dir)
{
	switch (dir)
	{
	case AdjacentDirs::E:
		return AdjacentDirs::W;
		break;
	case AdjacentDirs::W:
		return AdjacentDirs::E;
		break;
	case AdjacentDirs::S:
		return AdjacentDirs::N;
		break;
	case AdjacentDirs::N:
		return AdjacentDirs::S;
		break;
	case AdjacentDirs::NE:
		return AdjacentDirs::SW;
		break;
	case AdjacentDirs::NW:
		return AdjacentDirs::SE;
		break;
	case AdjacentDirs::SE:
		return AdjacentDirs::NW;
		break;
	case AdjacentDirs::SW:
		return AdjacentDirs::NE;
		break;
	}
	return AdjacentDirs::NoDir;
}

bool Utils::IsBonusTile(MovingTileTypes tileType)
{
	auto intType = tileType._to_integral();
	return intType == MovingTileTypes::RainbowObject
		|| intType == MovingTileTypes::ColumnBreakerObject
		|| intType == MovingTileTypes::RowBreakerObject
		|| intType == MovingTileTypes::XBreakerObject
		|| intType == MovingTileTypes::BombBreakerObject
		|| intType == MovingTileTypes::SeekerObject ;
}
