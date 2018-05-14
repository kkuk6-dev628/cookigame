#include "PieceSwapper.h"
#include "Controllers/ActionController.h"
#include "Controllers/BoardController.h"


PieceSwapper::PieceSwapper()
{
	swappingCells = new CellsList;
}


PieceSwapper::~PieceSwapper()
{
	CC_SAFE_DELETE(swappingCells);
}

void PieceSwapper::swap()
{
	if (!checkCells()) return;;

	Cell* prevCell = swappingCells->back();
	auto lastCell = prevCell;
	auto lastTile = prevCell->getMovingTile();
	for(auto cell : *swappingCells)
	{
		if(cell == lastCell) break;

		moveTileToCell(cell->getMovingTile(), prevCell);
		prevCell = cell;
	}
	moveTileToCell(lastTile, prevCell);
}

bool PieceSwapper::checkCells()
{
	for (auto cell : *swappingCells)
	{
		if (cell == nullptr || cell->isOutCell || cell->isFixed) return false;
	}
	return true;
}

void PieceSwapper::moveTileToCell(MovingTile* tile, Cell* cell)
{
	if(tile == nullptr)
	{
		cell->isEmpty = true;
		cell->isFillable = true;
		return;
	}
	tile->initMovingTile();
	auto showObj = PoolController::getInstance()->getTileShowObject();
	showObj->setSpriteFrame(tile->getSpriteFrame());
	showObj->setPosition(tile->getPosition());
	showObj->setAnchorPoint(Vec2(0.5, 0.5));
	if (tile->getParent() != nullptr && showObj->getParent() == nullptr)
	{
		tile->getParent()->addChild(showObj);
	}
	CKAction ckAction;
	ckAction.node = reinterpret_cast<Node*>(showObj);
	ckAction.action = ActionController::getInstance()->createPieceSwappingAction(cell->getBoardPos(), [=] {
		tile->setVisible(true);
		tile->isMoving = false;
		tile->movingDuration = 0.f;
		tile->setPosition(cell->getBoardPos());
		PoolController::getInstance()->recycleTileShowObject(showObj);
		BoardController::fallingTileCount--;
	}, ckAction.node);
	tile->setVisible(false);
	tile->isMoving = true;
	tile->movingDuration = static_cast<Sequence*>(ckAction.action)->getDuration();
	ActionController::getInstance()->pushAction(ckAction, true);
	BoardController::fallingTileCount++;

	cell->setSourceTile(tile);
}
