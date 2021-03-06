#pragma once
#include "BoardController.h"

class PathBoardController : public BoardController
{
public:
	PathBoardController();
	~PathBoardController();

	CREATE_FUNC(PathBoardController)

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode, Node* effectNode) override;

protected:

	void checkObjective() override {};
	void checkMoveCount() override;

	void addCellToBoard(char col, char row) override;
	void processCustomLogic(float) override;
	CellsList* findPath();
	Cell* getNextPath(Cell* cell);
	void initPathGame();

	void pathMoverCollected(Cell* cell) override;
	Cell* findSeekerTarget(CellsList* targetsList) const override;

	void movePathFollower();
	void showPathFollowerMovingAction(Cell* cell);
	void showPathFollowerMovingAction(CellsList* path);
	void showFrontCrushAction(Cell* frontCell);

	void spawnNewTile(Cell* cell) override;

private:
	CookieTile* pathFollowerObject = nullptr;
	Sprite* pathFollowerShow = nullptr;
	Cell* pathGoalCell = nullptr;

	char collectedPowerCount = 0;

	char flyingPowersCount = 0;
};

