#include "FallPath.h"



FallPath::FallPath()
{
}

FallPath::FallPath(Cell* endCell)
{
	this->endCell = endCell;
}

FallPath::FallPath(FallPath* other)
{
	endCell = other->endCell;
	startCell = other->startCell;
	for (auto itr = other->fallPath.rbegin(); itr != other->fallPath.rend(); ++itr)
	{
		fallPath.push_front(*itr);
	}
	containsPortal = other->containsPortal;
}


FallPath::~FallPath()
{
}

void FallPath::pushCell(Cell* cell)
{
	fallPath.push_front(cell);
	//if (cell->containsPortalOut())
	//{
	//	containsPortal = true;
	//}
}

void FallPath::setStartCell(Cell* cell)
{
	if(cell != nullptr)
	{
		if(startCell != nullptr && (cell->gridPos.Col != startCell->gridPos.Col || startCell->containsPortalIn() || startCell->containsPortalOut()))
		{
			pushCell(startCell);
		}
		startCell = cell;
	}
}

bool FallPath::containsCell(Cell* cell)
{
	if (cell == nullptr) return false;

	if (cell == startCell || cell == endCell) return true;

	return std::find(fallPath.begin(), fallPath.end(), cell) != fallPath.end();
}

void FallPath::addPath(FallPath* path)
{
	fallPath.push_front(path->endCell);
	for (auto itr = path->fallPath.rbegin(); itr != path->fallPath.rend(); ++itr)
	{
		fallPath.push_front(*itr);
	}
	startCell = path->startCell;
	if (path->containsPortal)
	{
		containsPortal = true;
	}
}

void FallPath::showFallAction()
{
	auto tile = startCell->getSpawnedTile();
	if(tile != nullptr)
	{
		tile->showFallAction(this);
	}
}

void FallPath::log()
{
	std::string logText = StringUtils::format("(%d, %d)", startCell->gridPos.Col, startCell->gridPos.Row);

	for(auto cell:fallPath)
	{
		logText += StringUtils::format(", (%d, %d)", cell->gridPos.Col, cell->gridPos.Row);
	}


}
