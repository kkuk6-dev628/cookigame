#include "Tile.h"
#include "General/Utils.h"
#include "General/Constants.h"
#include "Controllers/PoolController.h"

TileBase::TileBase()
{
	poolController = PoolController::getInstance(); 
}

void TileBase::initWithGrid(const char col, const char row)
{
	const auto pos = Utils::Grid2BoardPos(col, row);
	setContentSize(Size(CellSize, CellSize));
	setAnchorPoint(Vec2(0.5, 0.5));
	setPosition(pos.x, pos.y);
	gridPos.Col = col;
	gridPos.Row = row;
}

void TileBase::initTexture(const std::string textureName)
{
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);
	if(spriteFrame == nullptr)
	{
		return;
	}
	if(textureSprite == nullptr)
	{
		textureSprite = Sprite::create();
		addChild(textureSprite);
	}
	textureSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName));
	textureSprite->setContentSize(Size(CellSize, CellSize));
	textureSprite->setAnchorPoint(Vec2(0.5, 0.5));
	textureSprite->setPosition(CellSize / 2, CellSize / 2);
}

void BorderTile::initBorder(bool* borders)
{
	const auto borderThickness = CellSize / 12;

	if (borders[0] == true)
	{
		auto leftBorder = Sprite::createWithSpriteFrameName("border.png");
		
		leftBorder->setContentSize(Size(CellSize + borderThickness, borderThickness));
		leftBorder->setRotation(90);
		leftBorder->setAnchorPoint(Vec2(0.5, 0.5));
		leftBorder->setPosition(0, CellSize / 2);
		addChild(leftBorder);
	}
	if (borders[1] == true)
	{
		auto rightBorder = Sprite::createWithSpriteFrameName("border.png");

		rightBorder->setContentSize(Size(CellSize + borderThickness, borderThickness));
		rightBorder->setRotation(90);
		rightBorder->setAnchorPoint(Vec2(0.5, 0.5));
		rightBorder->setPosition(CellSize, CellSize / 2);
		addChild(rightBorder);
	}
	if (borders[2] == true)
	{
		auto bottomBorder = Sprite::createWithSpriteFrameName("border.png");

		bottomBorder->setContentSize(Size(CellSize + borderThickness, borderThickness));
		bottomBorder->setAnchorPoint(Vec2(0.5, 0.5));
		bottomBorder->setPosition(CellSize / 2, 0);
		addChild(bottomBorder);
	}
	if (borders[3] == true)
	{
		auto topBorder = Sprite::createWithSpriteFrameName("border.png");

		topBorder->setContentSize(Size(CellSize + borderThickness, borderThickness));
		topBorder->setAnchorPoint(Vec2(0.5, 0.5));
		topBorder->setPosition(CellSize / 2, CellSize);
		addChild(topBorder);
	}

}


