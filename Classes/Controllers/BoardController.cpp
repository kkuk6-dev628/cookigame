#include "BoardController.h"
#include "Models/BoardModels/BoardModel.h"
#include "LevelController.h"
#include "Models/BoardModels/BoardLayerModel.h"
#include "Models/BoardModels/Cell.h"
#include "proj.win32/Macros.h"
#include "General/Constants.h"
#include "General/Utils.h"
#include "Models/Tiles/Tile.h"

char BoardController::cellSize = 79;

BoardController::BoardController()
{
	currentLevel = LevelController::getInstance()->getCurrentLevel();
}


BoardController::~BoardController()
{
}


void BoardController::initWithJson(rapidjson::Value& data)
{
	if (this->boardModel != nullptr)
	{
		delete this->boardModel;
	}
	this->boardModel = new BoardModel();
	this->boardModel->initWithJson(data);

	initNode();
	initBackground();
}

void BoardController::initBackground()
{
	if (backgroundLayer != nullptr)
	{
		CC_SAFE_DELETE(backgroundLayer);
	}
	backgroundLayer = BoardLayer::create();
	backgroundLayer->initWithGrid(boardModel->Width, boardModel->Height);

	borderLayer = BoardLayer::create();
	borderLayer->initWithGrid(boardModel->Width, boardModel->Height);

	const auto matchLayer = boardModel->getMatchLayer();

	bool borders[4] = { false, false, false, false };

	for (char i = 0; i < boardModel->Height; i++)
	{
		for (char j = 0; j < boardModel->Width; j++)
		{
			const auto cell = matchLayer->getCell(j, i);
			if (cell->IsEmpty) continue;

			auto backgroundTile = TileBase::create();
			backgroundTile->initWithGrid(j, i);
			backgroundTile->initTexture("bg_tile.png");
			backgroundLayer->addChild(backgroundTile);

			auto hasBorder = false;
			for (char k = 0; k < 4; k++)
			{
				const char adjCol = j + AdjacentIndents[k][1];
				const char adjRow = i + AdjacentIndents[k][0];
				if (0 <= adjCol && adjCol < boardModel->Width 
					&& 0 <= adjRow && adjRow < boardModel->Height 
					&& !(matchLayer->getCell(adjCol, adjRow)->IsEmpty))
				{
					borders[k] = false;
				}
				else
				{
					borders[k] = true;
					hasBorder = true;
				}
			}

			if (hasBorder)
			{
				auto borderTile = BorderTile::create();
				borderTile->initWithGrid(j, i);
				borderTile->initBorder(borders);
				borderLayer->addChild(borderTile);
			}
		}
	}
	addChild(backgroundLayer);
	addChild(borderLayer);
	//auto obj = this->boardLayers->objectForKey(LayerId::Match);
	//assert(obj != nullptr, "Match layer not found!");
	//auto matchLayer = static_cast<BoardLayerModel*>(obj);
	//assert(matchLayer != nullptr, "Match layer is not valid!");
}

void BoardController::initNode()
{
	auto const width = CellSize * boardModel->Width;
	auto const height = CellSize * boardModel->Height;
	setContentSize(Size(width, height));
	const auto originX = CenterX - width / 2;
	const auto originY = CenterY - height / 2;
	setPosition(originX, originY);
}

void BoardController::update(float delta)
{
	Layer::update(delta);
}
