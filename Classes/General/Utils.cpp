#include "Utils.h"
#include "Constants.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}

GridSizeT Utils::StrToGridSize(std::string str, std::string delim)
{
	auto end = str.find(delim);
	assert(end != std::string::npos);
	auto strCol = str.substr(0, end);
	auto strRow = str.substr(end, str.length() - end - delim.length());
	GridSizeT gridSize;
	gridSize.Width = atoi(strCol.c_str());
	gridSize.Height = atoi(strRow.c_str());
	return gridSize;
}

GridPos Utils::StrToGridPos(std::string str, std::string delim)
{
	auto end = str.find(delim);
	assert(end != std::string::npos);
	auto strCol = str.substr(0, end);
	auto strRow = str.substr(end + delim.length(), str.length() - end - delim.length());
	GridPos gridPos;
	gridPos.Col = atoi(strCol.c_str());
	gridPos.Row = atoi(strRow.c_str());
	return gridPos;
}

cocos2d::Vec2 Utils::Grid2BoardPos(char col, char row, float boardWidth, float boardHeight)
{
	return cocos2d::Vec2(col * CellSize + CellSize / 2, row * CellSize + CellSize / 2);
}
