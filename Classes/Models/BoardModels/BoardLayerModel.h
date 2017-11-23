#pragma once
#include "base/CCRef.h"
#include "cocostudio/ActionTimeline/CCActionTimelineCache.h"

class Cell;
class CookieTile;

class BoardLayerModel : public cocos2d::Ref
{
public:
	~BoardLayerModel();
	
	static BoardLayerModel* create(char width, char height);
	static void RegisterTileClasses();

	void initWithJson(rapidjson::Value& json);

	Cell* getCell(const char col, const char row) const { return cells[row][col]; }

private:
	BoardLayerModel(char width, char height);

public:
	char Width;
	char Height;

private:
	Cell*** cells;
	CookieTile** tiles;
};

