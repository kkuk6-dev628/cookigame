#pragma once
#include "CookieTile.h"

class FallPath;
class ActionController;

class MovingTile :
	public CookieTile
{
public:
	MovingTile();
	~MovingTile();

	CREATE_FUNC(MovingTile);

	bool init() override;
	//void onEnterTransitionDidFinish() override;
	//bool onTouchBegan(Touch* touch, Event* unused_event);

	void showScaleBouncingAction() const;
	void showDirectionalScaleAction(AdjacentDirs dir) const;
	void showFallAction(FallPath* path);
	void showMoveAction(Cell* cell);

	bool isMovable() override;
	void showSwapAction(GridPos& gridPos, std::function<void()> callback);
	void initWithJson(rapidjson::Value& json) override;
	void initTexture() override;
	void initMovingTile();

	void setModifierTexture();
	SpriteFrame* getMainSpriteFrame() const
	{
		if (textureSprite == nullptr) return nullptr;
		return textureSprite->getSpriteFrame();
	}

	bool crush(bool showEffect) override;
	void showCrushEffect() override;

	bool isBonusTile() const
	{ 
		return movingTileType == +MovingTileTypes::BombBreakerObject || movingTileType == +MovingTileTypes::ColumnBreakerObject 
		|| movingTileType == +MovingTileTypes::RowBreakerObject || movingTileType == +MovingTileTypes::XBreakerObject || movingTileType == +MovingTileTypes::SeekerObject
		|| movingTileType == +MovingTileTypes::RainbowObject; 
	}

	bool isMoving = false;
	float movingDuration = 0.f;

protected:
	ActionController* actionController;
	MovingTileTypes movingTileType = MovingTileTypes::LayeredMatchObject;
	Sprite* modifierSprite = nullptr;
};

