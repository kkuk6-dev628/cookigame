#include "BoardLayerModel.h"
#include "Cell.h"
#include "Models/Tiles/Tile.h"
#include "General/Utils.h"
#include "Models/Tiles/LargeTile.h"
#include "Models/Tiles/LayeredMatchObject.h"
#include "Controllers/PoolController.h"


BoardLayerModel::BoardLayerModel(char width, char height)
{
	this->Width = width;
	this->Height = height;

	cells = new Cell**[height];
	tiles = new CookieTile*[height];
	for (auto i = 0; i < height; i++)
	{
		cells[i] = new Cell*[width];
		tiles[i] = new CookieTile[width];
		
		for (auto j = 0; j < width; j++)
		{
			cells[i][j] = new Cell();
			cells[i][j]->setGridPos(j, i);
			if (i > 0)
			{
				cells[i - 1][j]->upCell = cells[i][j];
				cells[i][j]->downCell = cells[i - 1][j];
			}
			if (j > 0)
			{
				cells[i][j - 1]->rightCell = cells[i][j];
				cells[i][j]->leftCell = cells[i][j - 1];
			}
		}
	}
	
}


BoardLayerModel::~BoardLayerModel()
{
	for (auto i = 0; i < Height; i++)
	{
		for (auto j = 0; j < Width; j++)
		{
			CC_SAFE_DELETE(cells[i][j]);
		}
		CC_SAFE_DELETE_ARRAY(cells[i]);
		CC_SAFE_DELETE_ARRAY(tiles[i]);
	}
	CC_SAFE_DELETE_ARRAY(cells);
	CC_SAFE_DELETE_ARRAY(tiles);
}

BoardLayerModel* BoardLayerModel::create(char width, char height)
{
	auto layerData = new BoardLayerModel(width, height);
	layerData->autorelease();
	return layerData;

}


void BoardLayerModel::initWithJson(rapidjson::Value& json)
{
	for (auto itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
	{
		if (itr->value.IsObject() && !itr->value.ObjectEmpty())
		{
			auto gridPos = Utils::StrToGridPos(itr->name.GetString(), "_");
			assert(this->Width > gridPos.Col && this->Height > gridPos.Row);
			const auto itr1 = itr->value.FindMember("type");
			if (itr1 != itr->value.MemberEnd() && itr1->value.IsString())
			{
				const auto typeName = itr1->value.GetString();
				auto tile = PoolController::getInstance()->getCookieTile(typeName);
				if (tile != nullptr)
				{
					tile->initWithGrid(gridPos.Col, gridPos.Row);
					tile->initWithJson(itr->value);

					this->cells[gridPos.Row][gridPos.Col]->setGridPos(gridPos.Col, gridPos.Row);
					this->cells[gridPos.Row][gridPos.Col]->setSourceTile(tile);
				}
			}
		}
	}

}

