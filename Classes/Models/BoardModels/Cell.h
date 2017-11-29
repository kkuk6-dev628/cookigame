#pragma once
#include "General/TypeDefs.h"

class CookieTile;

class Cell
{
public:
	Cell();
	~Cell();

	CookieTile* getSourceTile() const { return getTileAtLayer(LayerId::Match); }
	void setSourceTile(CookieTile* pTile);

	CookieTile* getTileAtLayer(LayerId layer) const;
	void setTileToLayer(CookieTile* pTile, LayerId layer);


	void setGridPos(const char col, const char row) { gridPos.Col = col; gridPos.Row = row; }
	void clear();
	GridPos gridPos;

	Cell* getFallCell() const;

	char fallDirection = 1; // top -> down, if -1 bottom -> up 
	bool isFixed = false;
	bool isFillable = true;
	bool isPass = false;

public:
	Cell* upCell = nullptr;
	Cell* downCell = nullptr;
	Cell* leftCell = nullptr;
	Cell* rightCell = nullptr;

private:
	cocos2d::__Dictionary* layers;
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

