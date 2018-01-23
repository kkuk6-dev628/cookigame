#pragma once
#include "Models/Tiles/CookieTile.h"
#include "Models/TilePools/TilePool.h"
#include "Models/TilePools/TileShowObjectPool.h"
#include "Models/TilePools/NodePool.h"
#include "Models/ShowObjects/AnimationShowObject.h"
#include "Models/Tiles/ChocolateChipObject.h"

class PoolController
{
public:
	static PoolController* getInstance();
	~PoolController();
	void RegisterTileClasses();

	CookieTile* getCookieTile(std::string typeName);
	void recycleCookieTile(CookieTile* cookieTile) const;

	//ChocolateChipObject* getChocolateChip() const;
	//void recycleChocolateChip(ChocolateChipObject* show) const;

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

	AnimationShowObject* getBlueSeekerShow() const;
	void recycleBlueSeekerShow(AnimationShowObject* anim) const;
	AnimationShowObject* getGreenSeekerShow() const;
	void recycleGreenSeekerShow(AnimationShowObject* anim) const;
	AnimationShowObject* getOrangeSeekerShow() const;
	void recycleOrangeSeekerShow(AnimationShowObject* anim) const;
	AnimationShowObject* getPurpleSeekerShow() const;
	void recyclePurpleSeekerShow(AnimationShowObject* anim) const;
	AnimationShowObject* getRedSeekerShow() const;
	void recycleRedSeekerShow(AnimationShowObject* anim) const;
	AnimationShowObject* getYellowSeekerShow() const;
	void recycleYellowSeekerShow(AnimationShowObject* anim) const;
	AnimationShowObject* getSeekerShow(TileColors color) const;
	void recycleSeekerShow(AnimationShowObject* show) const;

	AnimationShowObject* getChocolateCrushShow() const;
	void recycleChocolateCrushShow(AnimationShowObject* show) const;
	AnimationShowObject* getChocolateChipCrushShow() const;
	void recycleChocolateChipCrushShow(AnimationShowObject* show) const;

	AnimationShowObject* getDonutCrushShow() const;
	void recycleDonutCrushShow(AnimationShowObject* show) const;

	AnimationShowObject* getLineCrushShow() const;
	void recycleLineCrushShow(AnimationShowObject* show) const;

	AnimationShowObject* getBombAndLineCrushShow() const;
	void recycleBombAndLineCrushShow(AnimationShowObject* show) const;

private:
	PoolController();
	static PoolController* instance;

#pragma region LayeredMatchObject

	TilePool* tilesPool;
	TileShowObjectPool* tileShowObjectPool;

	NodePool<AnimationShowObject>* blueCrushShowPool;
	NodePool<AnimationShowObject>* greenCrushShowPool;
	NodePool<AnimationShowObject>* orangeCrushShowPool;
	NodePool<AnimationShowObject>* purpleCrushShowPool;
	NodePool<AnimationShowObject>* redCrushShowPool;
	NodePool<AnimationShowObject>* yellowCrushShowPool;

#pragma endregion

	NodePool<AnimationShowObject>* blueSeekerShowPool;
	NodePool<AnimationShowObject>* greenSeekerShowPool;
	NodePool<AnimationShowObject>* orangeSeekerShowPool;
	NodePool<AnimationShowObject>* purpleSeekerShowPool;
	NodePool<AnimationShowObject>* redSeekerShowPool;
	NodePool<AnimationShowObject>* yellowSeekerShowPool;

	NodePool<AnimationShowObject>* lineCrushShowPool;
	NodePool<AnimationShowObject>* bombAndLineCrushShowPool;

	NodePool<AnimationShowObject>* chocolateCrushPool;

	NodePool<ChocolateChipObject>* chocolateChipPool;
	NodePool<AnimationShowObject>* chocolateChipCrushPool;

	NodePool<DonutObject>* donutPool;
	NodePool<AnimationShowObject>* donutCrushPool;

};

