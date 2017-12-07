#pragma once
#include "platform/CCPlatformMacros.h"
#include "2d/CCLayer.h"
#include "deprecated/CCArray.h"
#include "json/document.h"
#include "Layers/BoardLayer.h"
#include "SpawnController.h"
#include "Models/BoardModels/Cell.h"
#include "Models/Tiles/MovingTile.h"
#include "Models/BoardModels/Match.h"
#include "PoolController.h"

class ActionController;
class BoardModel;
class Level;
class FallPath;
USING_NS_CC;

class BoardController : public Layer
{
public:
	BoardController();
	~BoardController();

	static char getCellSize() { return cellSize; };
	static void setCellSize(const char cs) { cellSize = cs; }

	bool init() override;
	void initControllersWithBoard() const;
	void initWithJson(rapidjson::Value& data);
	CREATE_FUNC(BoardController);

	Cell* getMatchCell(Vec2 nodePos) const;
	Cell* getMatchCell(GridPos& pos) const;
	Cell* getMatchCell(char col, char row) const;

	void onEnterTransitionDidFinish() override;
	bool onTouchBegan(Touch* touch, Event* unused_event) override;
	void onTouchMoved(Touch* touch, Event* unused_event) override;
	void onTouchEnded(Touch* touch, Event* unused_event) override;
	void onTouchCancelled(Touch* touch, Event* unused_event) override;

	void releaseWaitingMatch(int matchId) const;
	//void BoardController::initWithData(BoardModel* boardData);

protected:

#pragma region protected functions

	void processLogic(float);

	void initBoardElements();
	void addBackgroundTile(char col, char row) const;
	void initNode();
	int canSwapTiles(Cell* selectedCell, Cell* targetCell, bool addToCrush=true);
	Match* findMatch(Cell* startCell) const;
	void swapTilesInternal(Cell* selectedCell, Cell* targetCell) const;

	void doSomethingPerMove();
	int increaseMoveCount() { return ++moveCount; }
	int getMatchId() { return ++matchId; }

	void crushPendingCells();
	void crushMatch(Match* match);
	void fallTiles();
	FallPath* findFallPath(Cell* cell);
	void checkMatchesInBoard();

	void crushCell(Cell* pCell) const;

	////////////////////

#pragma endregion

#pragma region protected data members

	SpawnController* spawnController;
	ActionController* actionController;
	PoolController* poolController;

	static float centerX;
	static float centerY;
	static char cellSize;

	BoardModel* boardModel = nullptr;

	Level* currentLevel = nullptr;

	BoardLayer* backgroundLayer = nullptr;
	BoardLayer* borderLayer = nullptr;
	BoardLayer* layeredMatchLayer = nullptr;

	MovingTile* selectedTile = nullptr;
	MovingTile* targetTile = nullptr;
	__Array* pendingCrushCells;

	int moveCount = 0;
	int matchId = 0;

#pragma endregion

protected:
	void update(float delta) override;
};

