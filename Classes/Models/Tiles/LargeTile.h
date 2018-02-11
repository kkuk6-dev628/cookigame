#pragma once
#include "CookieTile.h"

class LargeTile : public CookieTile
{
public:
	LargeTile();
	~LargeTile();

protected:
	char colspan = 1;
	char rowspan = 1;
};


class ColorPie3Object : public LargeTile
{

};