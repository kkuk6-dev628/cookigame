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
#include "Models/BoardModels/BoardModel.h"
#include "ScoreController.h"
#include "SoundController.h"

class ActionController;
class BoardModel;
class Level;
class FallPath;
class LavaCakeObject;

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
	CREATE_FUNC(BoardController)

	virtual void initWithModel(BoardModel* model);
	virtual void initWithNode(Node* rootNode, Node* effectNode);

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

	float getBoardWidth() const { return boardModel->getWidth(); }
	float getBoardHeight() const { return boardModel->getHeight(); }

	void showGameWinDlg();
	void showGameFailedDlg();

protected:

#pragma region protected functions

	void processLogic(float);
	void checkFallingTileCount();
	virtual void processCustomLogic(float) {};
	virtual void checkObjective();
	virtual void checkMoveCount();
	virtual void finishLevel();

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
	void showRainbowLineEffect(Cell* refCell, Cell* targetCell);
	void crushDirectionalBreaker(Cell* cell, Direction direction);

	void combineSeekerAndBonus(Cell* seekerCell, Cell* bonusCell);
	void combineRainbowAndBonus(Cell* rainbowCell, Cell* bonusCell);
	void combine2Rainbow(Cell* first, Cell* second);
	void combineBombAndLine(Cell* refCell, Cell* targetCell);
	void combine2LineBreakers(Cell* refCell, Cell* targetCell);
	void combineXAndLine(Cell* refCell, Cell* targetCell);

	void crushMatch(Match* match);
	void crushAllCells();

	void addScore(ScoreType type, ScoreUnit val, char matchNumber, Vec2 pos) const;

	void showLineCrushEffect(Cell* cell, float rotation, char side = 0); // 0 : left and right, 1 : right, -1 : left
	
	void toggleLiquidFiller(std::string fillerType);

	void showBombAndLineCrushEffect(Cell* cell);
	void showBombCrushEffect(Cell* cell);

	bool checkPastHole(Cell* cell, char refCol, char refRow, bool inWater = false);
	Cell* fillCell(Cell* cell);
	bool fallTiles();
	void fallTilesLoop();
	FallPath* findFallPath(Cell* cell);
	virtual void spawnNewTile(Cell* cell);

	void checkMatchesInBoard();
	Match* findMatchInPendings(Match* newMatch);

	void doShuffle();
	void showShuffleAction();
	void shuffle(float);
	void showHintAction();

	virtual Cell* findSeekerTarget(CellsList* targetsList) const;
	void landingSeeker(AnimationShowObject* seekerShow, Cell* targetCell);
	void crushBonusManually(Cell* cell, std::string bonusString);

	virtual void crushCell(Cell* pCell, bool forceClear = false);
	void crushBombBreaker(Cell* cell);
	void crushTwoBomb(Cell* cell);
	void crushRowBreaker(Cell* cell, bool showEffect = true);
	void crushColumnBreaker(Cell* cell, bool showEffect = true);
	void crushXBreaker(Cell* cell);
	void crushSeeker(Cell* cell, MovingTileTypes bonusType=MovingTileTypes::LayeredMatchObject);
	void crushSeekerAndBonus(Cell* seekerCell, Cell* bonusCell);

	void crushNearbyCells(Cell* cell, TileColors color);
	virtual void crushUnderCells(Cell* cell);

	void processPendingSeekers();

	void initHintAction();
	void countDownMoveNumber();
	void updateMoveCountText();
	void fillLiquid(bool inverse = false);

	void moveConveyors();
	virtual void pathMoverCollected(Cell* cell){};

	void increaseObjectCount();

	void addLavaCakeObject(char col, char row, CookieTile* lavaCake);
	void spawnLavaCake(Cell* cell, CellsList* targets);
	void moveSpinners();

	void moveSwappers();

	void spreedHoneyModifier();

	void playCreateBonusSoundEffect(MovingTileTypes bonusType);

	void endGame();
	////////////////////

#pragma endregion

#pragma region protected data members

	SpawnController* spawnController;
	ActionController* actionController;
	PoolController* poolController;
	ScoreController* scoreController;
	SoundController* soundController;

	Node* rootNode = nullptr;
	Node* effectNode = nullptr;
	Node* topMenuArea = nullptr;
	Node* bottomMenuArea = nullptr;
	ui::Text* moveCountNode = nullptr;
	ui::Text* levelNumberNode = nullptr;
	ui::Text* objectCountNode = nullptr;
	ui::Text* scoreTextNode = nullptr;

	Vec2 objectTargetPos;
	Vec2 centerPos;

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

	float hintTime = HINT_TIME;

	float totalObjectCount = 0;
	float collectedObjectCount = 0;

	bool moveConveyorsFlag = false;
	bool moveSpinnerFlag = false;
	bool moveSwappersFlag = false;
	bool spreedHoneyModifierFlag = false;

	char checkFallingCount = 0;

#pragma endregion

protected:
	void update(float delta) override;

public:
};

