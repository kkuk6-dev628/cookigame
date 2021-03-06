#pragma once
#include "TypeDefs.h"
#include "Models/Tiles/CookieTile.h"

class Utils
{
public:
	Utils();
	~Utils();

	static bool containsCell(CellsList* cellsList, Cell* cell);

	static GridSizeT StrToGridSize(std::string str, std::string delim);
	static std::vector<std::string> splitString(std::string str, char delim);

	static GridPos StrToGridPos(std::string str, std::string delim);

	static cocos2d::Vec2 Grid2BoardPos(char col, char row, float boardWidth = 711, float boardHeight = 711);
	static cocos2d::Vec2 Grid2BoardPos(GridPos& gridPos);

	static GridPos Board2GridPos(cocos2d::Vec2 boardPos);
	static bool IsSameGrid(const GridPos gridA, const GridPos gridB);
	static AdjacentDirs getDirection(GridPos& posA, GridPos& posB);
	static AdjacentDirs getDirection(cocos2d::Vec2 offset);
	static GridPos getAdjacentPos(GridPos& pos, AdjacentDirs dir);
	static AdjacentDirs inverseDir(AdjacentDirs dir);
	static bool IsBonusTile(MovingTileTypes tileType);

	static Vec2 convertPos(Node* fromNode, Node* toNode);

	static bool checkSpawn(int spawnedTilesCount, float spawnPercent);

	static float calcAngle(Vec2 startPos, Vec2 endPos);
};

