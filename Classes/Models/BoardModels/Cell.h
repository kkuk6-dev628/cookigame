#pragma once
#include "General/TypeDefs.h"

class CookieTile;

class Cell
{
public:
	Cell();
	~Cell();

	CookieTile* getSourceTile() const { return pSourceTile; }
	void setSourceTile(CookieTile* pTile);
	void setGridPos(const char col, const char row) { gridPos.Col = col; gridPos.Row = row; }
	GridPos gridPos;

public:
	Cell* upCell = nullptr;
	Cell* downCell = nullptr;
	Cell* leftCell = nullptr;
	Cell* rightCell = nullptr;

private:
	CookieTile* pSourceTile = nullptr;
	bool isEmpty = true;


public:
	bool is_empty() const
	{
		return isEmpty;
	}

	void is_empty(bool isEmpty)
	{
		this->isEmpty = isEmpty;
	}

	__declspec(property(get = is_empty, put = is_empty)) bool IsEmpty;
};

