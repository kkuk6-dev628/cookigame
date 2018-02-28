#pragma once
#include "CookieTile.h"

class LargeTile : public CookieTile
{
public:
	LargeTile();
	~LargeTile();

	char getColSpan() const { return colspan; }
	char getRowSpan() const { return rowspan; }

protected:
	char colspan = 1;
	char rowspan = 1;
};


class ColorPie3Object : public LargeTile
{

};