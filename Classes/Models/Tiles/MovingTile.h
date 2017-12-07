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

	//void onEnterTransitionDidFinish() override;
	//bool onTouchBegan(Touch* touch, Event* unused_event);

	void showScaleBouncingAction() const;
	void showDirectionalScaleAction(AdjacentDirs dir) const;
	void showFallAction(FallPath* path);

	bool isMovable() const;
	void showSwapAction(GridPos& gridPos, std::function<void()> callback);
	void initWithJson(rapidjson::Value& json) override;
	void initTexture();

	void showCrushEffect() override;

protected:
	ActionController* actionController;
	MovingTileTypes movingTileType = MovingTileTypes::LayeredMatchObject;
};

