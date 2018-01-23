#pragma once
#include "BoardController.h"

class PathBoardController : public BoardController
{
public:
	PathBoardController();
	~PathBoardController();

	CREATE_FUNC(PathBoardController);

	void initWithModel(BoardModel* model) override;
	void initWithNode(Node* rootNode) override;

protected:
	void addCellToBoard(char col, char row) override;

	void initWaffleGame();

private:
	short totalWaffleCount = 0;

};

