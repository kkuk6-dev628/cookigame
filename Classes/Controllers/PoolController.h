#pragma once
#include "Models/Tiles/CookieTile.h"
#include "Models/TilePools/TilePool.h"
#include "Models/TilePools/TileShowObjectPool.h"
#include "Models/TilePools/NodePool.h"
#include "Models/ShowObjects/AnimationShowObject.h"

class PoolController
{
public:
	static PoolController* getInstance();
	~PoolController();
	void RegisterTileClasses();

	CookieTile* getCookieTile(std::string typeName);
	void recycleCookieTile(CookieTile* cookieTile) const;

	TileShowObject* getTileShowObject() const;
	void recycleTileShowObject(TileShowObject* showObj) const;

	AnimationShowObject* getMatchCrushShow(TileColors color) const;
	void recycleMatchCrushShow(AnimationShowObject* show, TileColors color) const;

	AnimationShowObject* getBlueCrushShow() const;
	void recycleBlueCrushShow(AnimationShowObject* anim) const;
	AnimationShowObject* getGreenCrushShow() const;
	void recycleGreenCrushShow(AnimationShowObject* anim) const;
	AnimationShowObject* getOrangeCrushShow() const;
	void recycleOrangeCrushShow(AnimationShowObject* anim) const;
	AnimationShowObject* getPurpleCrushShow() const;
	void recyclePurpleCrushShow(AnimationShowObject* anim) const;
	AnimationShowObject* getRedCrushShow() const;
	void recycleRedCrushShow(AnimationShowObject* anim) const;
	AnimationShowObject* getYellowCrushShow() const;
	void recycleYellowCrushShow(AnimationShowObject* anim) const;

private:
	PoolController();
	static PoolController* instance;

	TilePool* tilesPool;
	TileShowObjectPool* tileShowObjectPool;

	NodePool<AnimationShowObject>* blueCrushShowPool;
	NodePool<AnimationShowObject>* greenCrushShowPool;
	NodePool<AnimationShowObject>* orangeCrushShowPool;
	NodePool<AnimationShowObject>* purpleCrushShowPool;
	NodePool<AnimationShowObject>* redCrushShowPool;
	NodePool<AnimationShowObject>* yellowCrushShowPool;
};

