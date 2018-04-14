#include "HiderGroup.h"
#include "Controllers/ActionController.h"


HiderGroup::HiderGroup()
{
	segments = new std::vector<HiderSegmentObject*>;
	coveredCells = new CellsList;
}


HiderGroup::~HiderGroup()
{
	CC_SAFE_DELETE(segments);
	CC_SAFE_DELETE(coveredCells);
}

HiderSegmentObject* HiderGroup::getSegmentAt(char index) const
{
	if(segments == nullptr || segments->size() <= index)
	{
		return nullptr;
	}
	return segments->at(index);
}

void HiderGroup::initSegmentTextures()
{
	head->initTexture();
	for(auto seg : *segments)
	{
		seg->initTexture();
	}
}

void HiderGroup::moveHiderGroup(CellsList* cells, Cell* startCell)
{
	CC_ASSERT(cells->size() == segmentsCount);
	auto segment = segments->back();
	auto segmentsItr = segments->end();
	for(auto cell : *cells)
	{
		CellsList newVec;
		auto cellItr = std::find(cells->begin(), cells->end(), cell);
		std::copy(cells->begin(), cellItr, std::back_inserter(newVec));
		if(newVec.size() == 0)
		{
			newVec.push_back(cell);
		}
		moveSegmentToCell(segment, &newVec);
		if(segmentsItr == segments->begin())
		{
			segment = head;
		}
		else
		{
			--segmentsItr;
			segment = *segmentsItr;
		}
	}
	moveSegmentToCell(head, cells);
}

bool HiderGroup::checkHiderGroup()
{
	coveredCells->clear();
	for(auto segment : *segments)
	{
		auto cell = segment->getCell();
		if(cell->containsIceCover())
		{
			coveredCells->push_back(cell);
		}
	}

	if(coveredCells->size() > 0)
	{
		return false;
	}

	// else remove hider segment objects
	head->getCell()->removeTileAtLayer(LayerId::UnderCover);
	head->getCell()->clear();
	head->removeFromParent();

	for (auto segment : *segments)
	{
		auto cell = segment->getCell();
		segment->removeFromParent();
		cell->removeTileAtLayer(LayerId::UnderCover);
	}

	return true;
}

void HiderGroup::moveSegmentToCell(HiderSegmentObject* segment, CellsList* movePath)
{
	CKAction ckAction;
	auto actionController = ActionController::getInstance();
	ckAction.node = segment;
	ckAction.action = actionController->createHiderSegmentMoveAction(movePath, [=]()
	{
		segment->initTexture();
		if(segment->isHead())
		{
			auto headCell = movePath->back();
			auto oldTile = headCell->getSourceTile();
			if(oldTile != nullptr && oldTile->getParent() != nullptr)
			{
				oldTile->removeFromParent();
			}
			headCell->clear();
			auto iceCover = headCell->getTileAtLayer(LayerId::Cover);
			if(iceCover != nullptr)
			{
				iceCover->removeFromParent();
				headCell->removeTileAtLayer(LayerId::Cover);
				headCell->setSourceTile(segment);
			}
		}
	}, segment);
	actionController->pushAction(ckAction, false);
	auto oldCell = segment->getCell();
	oldCell->removeTileAtLayer(LayerId::UnderCover);
	auto newCell = movePath->back();
	newCell->setTileToLayer(segment, LayerId::UnderCover);
}
