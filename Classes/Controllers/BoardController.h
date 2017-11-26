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

class ActionController;
class BoardModel;
class Level;
USING_NS_CC;

class BoardController : public Layer
{
public:
	BoardController();
	~BoardController();

	static char getCellSize() { return cellSize; };
	static void setCellSize(const char cs) { cellSize = cs; }

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

	void releaseWaitingMatch(Node* tile);
	//void BoardController::initWithData(BoardModel* boardData);

protected:
	void initBoardElements();
	void addBackgroundTile(char col, char row) const;
	void initNode();
	int canSwapTiles(Cell* selectedCell, Cell* targetCell, bool addToCrush=true);
	Match* findMatch(Cell* startCell);
	void swapTilesInternal(Cell* selectedCell, Cell* targetCell);

	void doSomethingPerMove();
	int increaseMoveCount() { return ++moveCount; }
	int getMatchId() { return ++matchId; }

	SpawnController* spawnController;
	ActionController* actionController;

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
protected:
	void update(float delta) override;
};

