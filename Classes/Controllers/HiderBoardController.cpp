#include "HiderBoardController.h"
#include "Models/Tiles/HiderSegmentObject.h"
#include "ActionController.h"


HiderBoardController::HiderBoardController()
{
	hidersMap = new std::map<std::basic_string<char>, HiderGroup*>;
}


HiderBoardController::~HiderBoardController()
{
	CC_SAFE_DELETE(hidersMap);
}

void HiderBoardController::initWithModel(BoardModel* model)
{
	BoardController::initWithModel(model);
}

void HiderBoardController::initWithNode(Node* rootNode, Node* effectNode)
{
	BoardController::initWithNode(rootNode, effectNode);
	initHiderGame();
}

void HiderBoardController::initHiderGame()
{
	auto circleGroupNode = topMenuArea->getChildByName("top_menu_circle");
	circleGroupNode->getChildByName("FileNode_1")->setVisible(true);
	objectCountNode->setString(StringUtils::toString(totalObjectCount));
}

void HiderBoardController::processCustomLogic(float dt)
{
	checkHiders();
}

Cell* HiderBoardController::findSeekerTarget(CellsList* targetsList) const
{
	std::vector<Cell*> coveredSegmentCells;
	for(auto hiderGroupItem : *hidersMap)
	{
		auto groupCells = hiderGroupItem.second->getCoveredSegmentCells();
		coveredSegmentCells.insert(coveredSegmentCells.end(), groupCells->begin(), groupCells->end());
	}

	if(coveredSegmentCells.size() > 0)
	{
		Cell* retCell = nullptr;
		auto loopCount = 3;
		do
		{
			retCell = coveredSegmentCells.at(coveredSegmentCells.size() * rand_0_1());
			loopCount--;
		} while (Utils::containsCell(targetsList, retCell) && loopCount > 0);
		if (!Utils::containsCell(targetsList, retCell))
		{
			return retCell;
		}
	}
	return BoardController::findSeekerTarget(targetsList);
}


void HiderBoardController::addCellToBoard(char col, char row)
{
	BoardController::addCellToBoard(col, row);
	auto cell = getMatchCell(col, row);
	if (cell == nullptr || cell->isOutCell)
	{
		return;
	}

	auto hiderSegment = static_cast<HiderSegmentObject*>(cell->getTileAtLayer(LayerId::UnderCover));
	if (hiderSegment != nullptr && hiderSegment->isHead())
	{
		totalObjectCount++;
		auto seg = hiderSegment;
		auto pastSeg = hiderSegment;
		auto newSeg = hiderSegment;
		auto headCover = cell->getTileAtLayer(LayerId::Cover);
		if(headCover != nullptr && headCover->getParent() != nullptr)
		{
			headCover->removeFromParent();
			cell->removeTileAtLayer(LayerId::Cover);
		}
		cell->crushCell(false);
		cell->setSourceTile(hiderSegment);
		auto hiderGroup = new HiderGroup;
		hiderGroup->setHead(hiderSegment);
		do
		{
			pastSeg = seg;
			seg = newSeg;
			seg->setGroup(hiderGroup);
			newSeg = findNextSegment(seg, pastSeg);
			if(newSeg == nullptr) break;
			seg->setNextSegment(newSeg);
			newSeg->setPrevSegment(seg);

			hiderGroup->addSegment(newSeg);
		} while (!newSeg->isTail());

		hiderGroup->initSegmentTextures();
		auto test = findHiderMoveCells(seg->getCell(), hiderGroup->getSegmentsCount());
		hidersMap->insert(std::pair<std::string, HiderGroup*>(hiderSegment->getChain(), hiderGroup));
	}
}

HiderSegmentObject* HiderBoardController::findNextSegment(HiderSegmentObject* seg, HiderSegmentObject* pastSeg) const
{
	auto cell = seg->getCell();
	for(auto indent : AdjacentIndents)
	{
		auto adjCell = getMatchCell(cell->gridPos.Col + indent[1], cell->gridPos.Row + indent[0]);
		if(adjCell == nullptr || adjCell->isOutCell || !adjCell->containsPopsicle()) continue;
		auto hiderSeg = static_cast<HiderSegmentObject*>(adjCell->getTileAtLayer(LayerId::UnderCover));
		if(hiderSeg != nullptr && hiderSeg != pastSeg && hiderSeg->getChain() == seg->getChain())
		{
			return hiderSeg;
		}
	}
	return nullptr;
}

void HiderBoardController::crushUnderCells(Cell* cell)
{
	if (cell->crushUnderTiles(LayerId::Cover))
	{
		//canCollectedPopsicle(cell);
	}
}

CellsList* HiderBoardController::findHiderMoveCells(Cell* startCelll, char segmentsCount)
{
	auto hiderMoveCells = new CellsList;
	auto res = searchCoveredCells(startCelll, nullptr, hiderMoveCells, segmentsCount);
	return hiderMoveCells;
}

bool HiderBoardController::searchCoveredCells(Cell* cell, Cell* exceptCell, CellsList* coveredCells, char count)
{
	if (cell == nullptr || cell->isOutCell) return false;

	if (count <= 0) return true;

	for (auto adj : AdjacentIndents)
	{
		auto adjCell = getMatchCell(cell->gridPos.Col + adj[1], cell->gridPos.Row + adj[0]);
		if (adjCell == nullptr || adjCell == exceptCell || adjCell->isOutCell || adjCell->hiderSearch || !adjCell->containsIceCover() || adjCell->containsPopsicle()) continue;
		adjCell->hiderSearch = true;
		auto res = searchCoveredCells(adjCell, cell, coveredCells, count - 1);
		adjCell->hiderSearch = false;
		if (res)
		{
			coveredCells->push_front(adjCell);
			return true;
		}
	}
	return false;
}

void HiderBoardController::checkHiders()
{
	if (fallingTileCount > 0 || gameState != Idle) return;

	std::list<std::string> removeHiders;
	for(auto hiderItem : *hidersMap)
	{
		if (hiderItem.second->checkHiderGroup())
		{
			removeHiders.push_back(hiderItem.first);
			auto pos = hiderItem.second->getHead()->getPosition();
			showHiderCollectingAction(pos);
		}
	}

	for(auto removeKey : removeHiders)
	{
		hidersMap->erase(removeKey);
	}
}

void HiderBoardController::showHiderCollectingAction(Vec2& pos)
{
	auto showObj = poolController->getHiderShow();
	showObj->setPosition(pos);
	effectNode->addChild(showObj);
	CKAction ckAction;
	ckAction.node = showObj;
	ckAction.action = actionController->createHiderCollectionAction(objectTargetPos, [=] {
		this->increaseObjectCount();
		this->poolController->recycleHiderShow(showObj);
	}, showObj);
	actionController->pushAction(ckAction, true);
}


void HiderBoardController::moveHider(HiderSegmentObject* headSeg)
{
	auto nextSeg = headSeg->getNextSegment();
	auto segmentsCount = nextSeg->getGroup()->getSegmentsCount();
	Cell* iceCoverCell = nullptr;
	do
	{
		if(nextSeg->getCell()->containsIceCover())
		{
			if(iceCoverCell == nullptr)
			{
				iceCoverCell = nextSeg->getCell();
			}
			auto cells = findHiderMoveCells(nextSeg->getCell(), segmentsCount);
			if(cells->size() == segmentsCount)
			{
				headSeg->getCell()->clear();
				headSeg->getGroup()->moveHiderGroup(cells, nextSeg->getCell());
				return;
			}
		}
		nextSeg = nextSeg->getNextSegment();
	} while (nextSeg != nullptr);

	if (iceCoverCell != nullptr)
	{
		iceCoverCell->crushUnderTiles(LayerId::Cover);
	}
	//headSeg->showShakeAction();
}

void HiderBoardController::crushCell(Cell* pCell)
{
	if (pCell == nullptr || pCell->isOutCell) return;
	
	auto hiderHead = reinterpret_cast<HiderSegmentObject*>(pCell->getMovingTile());
	if(hiderHead != nullptr && hiderHead->isHead())
	{
		moveHider(hiderHead);
	}
	else
	{
		BoardController::crushCell(pCell);
	}
}
