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
#include "ui/UIText.h"

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
	void initWithJson(rapidjson::Value& data);
	CREATE_FUNC(BoardController);

	virtual void initWithModel(BoardModel* model);
	virtual void initWithNode(Node* rootNode);

	GoalTypes getGoalType();

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
	void manualShuffle();
	//void BoardController::initWithData(BoardModel* boardData);

protected:

#pragma region protected functions

	void processLogic(float);

	void initBoardElements();
	void initBoardLayers();
	void initLiquidLayer();
	void initAnimations();

	void addMask();
	void addTileToMask(char col, char row);
	void addTileToLiquidMask(GridPos& gridPos);

	virtual void addCellToBoard(char col, char row);

	BoardLayer* getBoardLayer(LayerId layerId);

	void addBackgroundTile(char col, char row);
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
	void crushDirectionalBreaker(Cell* cell, Direction direction);

	void combineSeekerAndBonus(Cell* seekerCell, Cell* bonusCell);
	void combineRainbowAndBonus(Cell* rainbowCell, Cell* bonusCell);
	void combineBombAndLine(Cell* refCell, Cell* targetCell);
	void combine2LineBreakers(Cell* refCell, Cell* targetCell);
	void combineXAndLine(Cell* refCell, Cell* targetCell);

	void crushMatch(Match* match);

	void showLineCrushEffect(Cell* cell, float rotation);
	void showBombAndLineCrushEffect(Cell* cell);

	bool checkPastHole(Cell* cell, char refCol, char refRow, bool inWater = false);
	Cell* fillCell(Cell* cell);
	bool fallTiles();
	void fallTilesLoop();
	FallPath* findFallPath(Cell* cell);
	void checkMatchesInBoard();
	Match* findMatchInPendings(Match* newMatch);

	void doShuffle();
	void showShuffleAction();
	void shuffle(float);

	Cell* findSeekerTarget(std::list<Cell*>* targetsList);
	void landingSeeker(AnimationShowObject* seekerShow, Cell* targetCell);
	void crushBonusManually(Cell* cell, std::string bonusString);

	void crushCell(Cell* pCell);
	void crushBombBreaker(Cell* cell);
	void crushRowBreaker(Cell* cell);
	void crushColumnBreaker(Cell* cell);
	void crushXBreaker(Cell* cell);
	void crushSeeker(Cell* cell, MovingTileTypes bonusType=MovingTileTypes::LayeredMatchObject);

	void crushNearbyCells(Cell* cell);
	void processPendingSeekers();

	void fillLiquid(bool inverse = false);

	////////////////////

#pragma endregion

#pragma region protected data members

	SpawnController* spawnController;
	ActionController* actionController;
	PoolController* poolController;

	Node* rootNode = nullptr;
	Node* topMenuArea = nullptr;
	Node* bottomMenuArea = nullptr;
	ui::Text* moveCountNode = nullptr;
	ui::Text* levelNumberNode = nullptr;
	ui::Text* objectCountNode = nullptr;
	ui::Text* scoreTextNode = nullptr;

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

	AnimationShowObject* shuffleAnimation = nullptr;

	DrawNode* stencil = nullptr;
	ClippingNode* maskNode = nullptr;
	
	Node* liquidNode = nullptr;
	DrawNode* liquidStencil = nullptr;
	ClippingNode* liquidMask = nullptr;

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

