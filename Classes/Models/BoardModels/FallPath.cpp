#include "FallPath.h"



FallPath::FallPath()
{
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
