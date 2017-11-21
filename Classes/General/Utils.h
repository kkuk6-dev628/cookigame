#pragma once
#include "TypeDefs.h"

class Utils
{
public:
	Utils();
	~Utils();
	static GridSizeT StrToGridSize(std::string str, std::string delim);
	static GridPos StrToGridPos(std::string str, std::string delim);

};

