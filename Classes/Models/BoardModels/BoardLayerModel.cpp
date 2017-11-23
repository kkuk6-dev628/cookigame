#include "BoardLayerModel.h"
#include "Cell.h"
#include "Models/Tiles/Tile.h"
#include "General/Utils.h"
#include "Models/Tiles/LargeTile.h"

static factory TileClassFactory;
#define REGISTER_CLASS(n) TileClassFactory.register_class<n>(#n)

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

void BoardLayerModel::RegisterTileClasses()
{
	REGISTER_CLASS(LayeredMatchObject);
	REGISTER_CLASS(WaffleObject);
	REGISTER_CLASS(ColorPie3Object);

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
				const auto tile = static_cast<CookieTile*>(TileClassFactory.construct(typeName));
				if (tile != nullptr)
				{
					this->cells[gridPos.Row][gridPos.Col]->setSourceTile(tile);
				}
			}
		}
	}

}

