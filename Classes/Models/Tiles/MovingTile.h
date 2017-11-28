#pragma once
#include "CookieTile.h"
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

	bool isMovable() const;
	void showSwapAction(GridPos& gridPos, std::function<void()> callback);
	void initWithJson(rapidjson::Value& json) override;

protected:
	MovingTileTypes movingTileType = MovingTileTypes::LayeredMatchObject;
};

