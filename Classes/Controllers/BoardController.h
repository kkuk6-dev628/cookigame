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
	static int fallingTileCount;
	static GameState gameState;

	bool init() override;
	void initWithModel(BoardModel* model);

	GoalTypes getGoalType();

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

	void addTile(char col, char row, MovingTileTypes type, TileColors tileColor);
	void addMovingTile(Cell* cell, MovingTileTypes type, TileColors tileColor);
	//void BoardController::initWithData(BoardModel* boardData);

protected:

#pragma region protected functions

	void processLogic(float);

	void initBoardElements();
	void initBoardLayers();
	BoardLayer* getBoardLayer(LayerId layerId);

	void addBackgroundTile(char col, char row) const;
	void initNode();
	int canSwapTiles(Cell* selectedCell, Cell* targetCell, bool addToCrush=true);
	Match* findMatch(Cell* startCell);
	void setDirtyCell(char col, char row);
	void swapTilesInternal(Cell* selectedCell, Cell* targetCell) const;

	void doSomethingPerMove();
	int increaseMoveCount() { return ++moveCount; }
	int getMatchId() { return ++matchId; }

	void crushPendingCells();
	void crushNormalMatch(Match* match);
	void crushBonusMatch(Match* match);
	void crushRainbowMatch(Match* match);
	void combineRainbowAndBonus(Cell* rainbowCell, Cell* bonusCell);
	void combineBombAndLine(Cell* refCell);
	void crushMatch(Match* match);

	bool checkPastHole(Cell* cell, char refCol, char refRow, bool inWater = false);
	Cell* fillCell(Cell* cell);
	void fallTiles();
	FallPath* findFallPath(Cell* cell);
	void checkMatchesInBoard();

	Cell* findSeekerTarget(std::list<Cell*>* targetsList);
	void landingSeeker(AnimationShowObject* seekerShow, Cell* targetCell);

	void crushCell(Cell* pCell);
	void crushBombBreaker(Cell* cell);
	void crushRowBreaker(Cell* cell);
	void crushColumnBreaker(Cell* cell);
	void crushXBreaker(Cell* cell);
	void crushSeeker(Cell* cell);

	void dualXCrush(Cell* cell);

	void processPendingSeekers();

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

	BoardLayer* backgroundLayer		= nullptr;
	BoardLayer* borderLayer			= nullptr;
	BoardLayer* underCoverLayer		= nullptr;
	BoardLayer* coverLayer			= nullptr;
	BoardLayer* pathConveyerLayer	= nullptr;
	BoardLayer* waffleLayer			= nullptr;
	BoardLayer* layeredMatchLayer	= nullptr;
	BoardLayer* toppllerLayer		= nullptr;
	BoardLayer* targetLayer			= nullptr;
	BoardLayer* portalLayer			= nullptr;
	BoardLayer* showObjectsLayer	= nullptr;

	__Dictionary* layersDict = nullptr;

	MovingTile* selectedTile = nullptr;
	MovingTile* targetTile = nullptr;
	__Array* pendingCrushCells;
	__Array* pendingSeekers;

	int moveCount = 0;
	int matchId = 0;

#pragma endregion

protected:
	void update(float delta) override;

public:
};

