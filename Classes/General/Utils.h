#pragma once
#include "TypeDefs.h"

class Utils
{
public:
	Utils();
	~Utils();
	
	static GridSizeT StrToGridSize(std::string str, std::string delim);
	
	static GridPos StrToGridPos(std::string str, std::string delim);

	static cocos2d::Vec2 Grid2BoardPos(char col, char row, float boardWidth = 711, float boardHeight = 711);
};

