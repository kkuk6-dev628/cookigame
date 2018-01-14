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

	bool isMovable() const;
	void showSwapAction(GridPos& gridPos, std::function<void()> callback);
	void initWithJson(rapidjson::Value& json) override;
	void initTexture() override;

	bool crush(bool showEffect) override;
	void showCrushEffect() override;

	bool isMoving = false;
	float movingDuration = 0.f;

protected:
	ActionController* actionController;
	MovingTileTypes movingTileType = MovingTileTypes::LayeredMatchObject;
};

