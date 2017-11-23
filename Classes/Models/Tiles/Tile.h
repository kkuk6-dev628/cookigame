#pragma once
#include "2d/CCSprite.h"

USING_NS_CC;

class Cell;

class TileBase : public Node
{
public:
	CREATE_FUNC(TileBase);
	void initWithGrid(char col, char row);
	virtual void initTexture(std::string textureName);

	char Col = 0;
	char Row = 0;

protected:
	Sprite* textureSprite = nullptr;
};

class BorderTile : public TileBase
{
public:
	CREATE_FUNC(BorderTile);
	void initBorder(bool* borders);
};