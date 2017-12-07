#pragma once
#include "2d/CCSprite.h"
#include "General/TypeDefs.h"

USING_NS_CC;

class Cell;
class PoolController;

class TileBase : public Node
{
public:
	TileBase();

	CREATE_FUNC(TileBase)
	void initWithGrid(char col, char row);
	virtual void initTexture(std::string textureName);

	GridPos gridPos;

protected:
	Sprite* textureSprite = nullptr;
	PoolController* poolController = nullptr;

};

class BorderTile : public TileBase
{
public:
	CREATE_FUNC(BorderTile)
	void initBorder(bool* borders);
};