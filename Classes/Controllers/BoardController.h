#pragma once
#include "platform/CCPlatformMacros.h"
#include "2d/CCLayer.h"
#include "deprecated/CCArray.h"
#include "json/document.h"
#include "Layers/BoardLayer.h"

class BoardModel;
class Level;
USING_NS_CC;

class BoardController : public Layer
{
public:
	BoardController();
	~BoardController();

	static char getCellSize() { return cellSize; };
	static void setCellSize(const char cs) { cellSize = cs; }

	void initWithJson(rapidjson::Value& data);
	CREATE_FUNC(BoardController);

	//void BoardController::initWithData(BoardModel* boardData);

protected:
	void initBackground();
	void initNode();

	static float centerX;
	static float centerY;
	static char cellSize;

	BoardModel* boardModel = nullptr;

	Level* currentLevel = nullptr;

	BoardLayer* backgroundLayer = nullptr;
	BoardLayer* borderLayer = nullptr;

protected:
	void update(float delta) override;
};

