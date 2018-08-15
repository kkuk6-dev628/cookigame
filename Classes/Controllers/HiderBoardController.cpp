#include "HiderBoardController.h"
#include "Models/Tiles/HiderSegmentObject.h"
#include "ActionController.h"
#include "Models/DataModels/Level.h"


HiderBoardController::HiderBoardController()
{
	hidersMap = new std::map<std::basic_string<char>, HiderGroup*>;
	pendingHiders = new std::list<HiderSegmentObject *>;
}


HiderBoardController::~HiderBoardController()
{
	CC_SAFE_DELETE(hidersMap);
	CC_SAFE_DELETE(pendingHiders);
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
	processPendingHiders();
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
		} while (Utils::containsCell(targetsList, retCell) && loopCount > 0 && retCell->isEmpty);
		if (!Utils::containsCell(targetsList, retCell) && !retCell->isEmpty)
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
		auto tile = cell->getMovingTile();
		if(tile != nullptr) tile->removeFromParent();
		cell->clear();
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
		//auto test = findHiderMoveCells(seg->getCell(), hiderGroup->getSegmentsCount());
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

bool HiderBoardController::findPathToTarget(Cell* startCell, Cell* exceptCell, CellsList* path, Cell* targetCell)
{
	std::queue<Coord*> q;
	auto curCell = startCell;
	auto curDist = 1;
	q.push(new Coord(curCell, curDist, path));
	int check[10][10] = {0};
	while(!q.empty())
	{
		auto coord = q.front();
		curCell = coord->cell;
		auto curPath = &coord->path;
		curDist = coord->dist;
		q.pop();
		if (curCell == nullptr || curCell->isOutCell) continue;
		if (curCell == targetCell) 
		{
			path->insert(path->begin(), curPath->begin(), curPath->end());
			path->reverse();
			return true;
		}

		for (auto adj : AdjacentIndents)
		{
			auto adjCol = curCell->gridPos.Col + adj[1];
			auto adjRow = curCell->gridPos.Row + adj[0];

			auto adjCell = getMatchCell(adjCol, adjRow);

			if (adjCell == nullptr || adjCell == exceptCell || adjCell->isOutCell || !adjCell->containsIceCover() 
				|| adjCell->containsPopsicle() || check[adjRow][adjCol] != 0) continue;

			q.push(new Coord(adjCell, curDist + 1, curPath));
			check[adjRow][adjCol] = check[curCell->gridPos.Row][curCell->gridPos.Col] + 1;
		}

		CC_SAFE_DELETE(coord);

	}

	return false;
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

	if (count <= 0 && cell->canPlaceHiderHead()) return true;

	for (auto adj : AdjacentIndents)
	{
		auto adjCell = getMatchCell(cell->gridPos.Col + adj[1], cell->gridPos.Row + adj[0]);

		if (adjCell == nullptr || adjCell == exceptCell || adjCell->isOutCell || adjCell->hiderSearch 
			|| !adjCell->containsIceCover() || adjCell->containsPopsicle()) continue;

		adjCell->hiderSearch = true;
		auto res = searchCoveredCells(adjCell, cell, coveredCells, count - 1);
		adjCell->hiderSearch = false;
		if (res)
		{
			coveredCells->push_back(adjCell);
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
		auto pos = hiderItem.second->getHead()->getPosition();
		if (hiderItem.second->checkHiderGroup())
		{
			removeHiders.push_back(hiderItem.first);
			showHiderCollectingAction(pos);
		}
	}

	for(auto removeKey : removeHiders)
	{
		hidersMap->erase(removeKey);
	}
}

void HiderBoardController::checkMoveCount()
{
	if (fallingTileCount > 0 || gameState != Idle) return;
	if (pendingSeekers->count() > 0) return;
	if (this->movingObjectiveCount > 0) return;
	if (this->pendingHiders->size() > 0) return;

	if (totalObjectCount == collectedObjectCount)
	{
		finishLevel();
	}
	else if (currentLevel->getMoveCount() <= moveCount)
	{
		gameState = Failed;
		showGameFailedDlg();
	}
}

CellsList* HiderBoardController::getSeekerTargets(int count) const
{
	std::vector<Cell*> coveredSegmentCells;
	for (auto hiderGroupItem : *hidersMap)
	{
		auto groupCells = hiderGroupItem.second->getCoveredSegmentCells();
		coveredSegmentCells.insert(coveredSegmentCells.end(), groupCells->begin(), groupCells->end());
	}
	auto targets = new CellsList;
	for (auto i = 0; i < count && coveredSegmentCells.size() > 0; i++)
	{
		int j = rand_0_1() * coveredSegmentCells.size();

		auto cell = coveredSegmentCells.at(j);

		targets->push_back(cell);
		coveredSegmentCells.erase(coveredSegmentCells.begin() + j);
	}

	if(targets->size() < count)
	{
		auto temp = BoardController::getSeekerTargets(count - targets->size());
		targets->insert(targets->end(), temp->begin(), temp->end());
		CC_SAFE_DELETE(temp);
	}
	return targets;
}


void HiderBoardController::processPendingHiders()
{
	if (fallingTileCount > 0 || gameState != Idle) return;

	for (auto hider : *pendingHiders)
	{
		moveHider(hider);
	}

	pendingHiders->clear();
}

void HiderBoardController::showHiderCollectingAction(Vec2& pos)
{
	auto showObj = poolController->getHiderShow();
	showObj->setPosition(pos);
	effectNode->addChild(showObj);
	CKAction ckAction;
	ckAction.node = showObj;
	this->movingObjectiveCount++;
	ckAction.action = actionController->createHiderCollectionAction(objectTargetPos, [=] {
		this->increaseObjectCount();
		this->poolController->recycleHiderShow(showObj);
		this->movingObjectiveCount--;
	}, showObj);
	actionController->pushAction(ckAction, true);
}


void HiderBoardController::moveHider(HiderSegmentObject* headSeg)
{
	auto nextSeg = headSeg->getNextSegment();
	if (nextSeg == nullptr) return;

	auto group = nextSeg->getGroup();
	char segmentsCount = 2;
	if (group != nullptr)
	{
		segmentsCount = group->getSegmentsCount();
	}

	Cell* iceCoverCell = nullptr;

	if(boardModel->getHiderTargets() != nullptr && boardModel->getHiderTargets()->find(headSeg->getChain()) != boardModel->getHiderTargets()->end())
	{
		do
		{
			if (nextSeg->getCell()->containsIceCover())
			{
				if (iceCoverCell == nullptr)
				{
					iceCoverCell = nextSeg->getCell();
				}
				auto path = new CellsList;
				auto res = findPathToTarget(nextSeg->getCell(), nullptr, path, boardModel->getHiderTargets()->at(headSeg->getChain()));

				if (res && path->size() > 0)
				{
					headSeg->getCell()->clear();
					headSeg->getGroup()->moveHiderGroup(path, nextSeg->getCell());
					boardModel->removeHiderTarget(headSeg->getChain());
					CC_SAFE_DELETE(path);
					return;
				}
				CC_SAFE_DELETE(path);
			}
			nextSeg = nextSeg->getNextSegment();
		} while (nextSeg != nullptr);
	}
	else
	{
		do
		{
			if (nextSeg->getCell()->containsIceCover())
			{
				if (iceCoverCell == nullptr)
				{
					iceCoverCell = nextSeg->getCell();
				}
				auto cells = findHiderMoveCells(nextSeg->getCell(), segmentsCount);
				if (cells->size() >= segmentsCount)
				{
					headSeg->getCell()->clear();
					headSeg->getGroup()->moveHiderGroup(cells, nextSeg->getCell());
					CC_SAFE_DELETE(cells);
					return;
				}
				CC_SAFE_DELETE(cells);
			}
			nextSeg = nextSeg->getNextSegment();
		} while (nextSeg != nullptr);
	}

	if (iceCoverCell != nullptr)
	{
		iceCoverCell->crushUnderTiles(LayerId::Cover);
		checkHiders();
	}
	//headSeg->showShakeAction();
}

void HiderBoardController::crushCell(Cell* pCell, bool forceClear)
{
	if (pCell == nullptr || pCell->isOutCell) return;
	
	auto hiderHead = reinterpret_cast<HiderSegmentObject*>(pCell->getMovingTile());
	if(hiderHead != nullptr && hiderHead->isHead())
	{
		if (std::find(pendingHiders->begin(), pendingHiders->end(), hiderHead) == pendingHiders->end())
		{
			pendingHiders->push_back(hiderHead);
		}
	}
	else
	{
		BoardController::crushCell(pCell, forceClear);
	}
}
