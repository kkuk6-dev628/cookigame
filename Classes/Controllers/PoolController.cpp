#include "PoolController.h"
#include "Models/Tiles/LayeredMatchObject.h"
#include "Models/Tiles/LargeTile.h"
#include "Models/Tiles/FixTiles.h"
#include "Models/Tiles/SpawnerObject.h"

PoolController* PoolController::instance = nullptr;
static factory TileClassFactory;
#define REGISTER_CLASS(n) TileClassFactory.register_class<n>(#n)

PoolController::PoolController()
{
	tilesPool = new TilePool;
	tileShowObjectPool = new TileShowObjectPool;
	blueCrushShowPool = new NodePool<AnimationShowObject>;
	greenCrushShowPool = new NodePool<AnimationShowObject>;
	orangeCrushShowPool = new NodePool<AnimationShowObject>;
	purpleCrushShowPool = new NodePool<AnimationShowObject>;
	redCrushShowPool = new NodePool<AnimationShowObject>;
	yellowCrushShowPool = new NodePool<AnimationShowObject>;
	RegisterTileClasses();
}


PoolController* PoolController::getInstance()
{
	if (instance == nullptr)
		instance = new PoolController();
	return instance;
}

PoolController::~PoolController()
{
	delete tilesPool;
}

void PoolController::RegisterTileClasses()
{
	REGISTER_CLASS(LayeredMatchObject);
	REGISTER_CLASS(WaffleObject);
	REGISTER_CLASS(ColorPie3Object);
	REGISTER_CLASS(SpawnerObject);
	REGISTER_CLASS(CrackerObject);
	//REGISTER_CLASS(ColorPie3Object);

}

CookieTile* PoolController::getCookieTile(std::string typeName)
{
	CookieTile* cookieTile;
	if (MovingTileTypes::_is_valid_nocase(typeName.c_str()))
	{
		if (tilesPool->size() > 0)
		{
			cookieTile = tilesPool->getTile();
		}
		else
		{
			cookieTile = new MovingTile();
		}
	}
	else
	{
		cookieTile = static_cast<CookieTile*>(TileClassFactory.construct(typeName));
	}
	if (cookieTile != nullptr) 
	{
		cookieTile->getType() = typeName;
	}
	return cookieTile;
}

void PoolController::recycleCookieTile(CookieTile* cookieTile) const
{
	if (MovingTileTypes::_is_valid_nocase(cookieTile->getType().c_str()))
	{
		tilesPool->putTile(static_cast<MovingTile*>(cookieTile));
	}
	else
	{
		CC_SAFE_RELEASE(cookieTile);
	}

}

TileShowObject* PoolController::getTileShowObject() const
{
	if (tileShowObjectPool->size() > 0)
	{
		return tileShowObjectPool->getTileShow();
	}
	else
	{
		return new TileShowObject();
	}
}

void PoolController::recycleTileShowObject(TileShowObject* showObj) const
{
	tileShowObjectPool->putTileShow(showObj);
}

AnimationShowObject* PoolController::getBlueCrushShow() const
{
	AnimationShowObject* show;
	if(blueCrushShowPool->size() > 0)
	{
		show = blueCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_blue.csb");
	}
	show->reuse([=]()
	{
		this->recycleBlueCrushShow(show);
	});
	return show;
}

void PoolController::recycleBlueCrushShow(AnimationShowObject* anim) const
{
	anim->recycle();
	blueCrushShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getGreenCrushShow() const
{
	AnimationShowObject* show;
	if (greenCrushShowPool->size() > 0)
	{
		show = greenCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_green.csb");
	}
	show->reuse([=]()
	{
		this->recycleGreenCrushShow(show);
	});
	return show;
}

void PoolController::recycleGreenCrushShow(AnimationShowObject* anim) const
{
	anim->recycle();
	greenCrushShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getOrangeCrushShow() const
{
	AnimationShowObject* show;
	if (orangeCrushShowPool->size() > 0)
	{
		show = orangeCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_orange.csb");
	}
	show->reuse([=]()
	{
		this->recycleOrangeCrushShow(show);
	});
	return show;
}

void PoolController::recycleOrangeCrushShow(AnimationShowObject* anim) const
{
	anim->recycle();
	orangeCrushShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getPurpleCrushShow() const
{
	AnimationShowObject* show;
	if (purpleCrushShowPool->size() > 0)
	{
		show = purpleCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_purple.csb");
	}
	show->reuse([=]()
	{
		this->recyclePurpleCrushShow(show);
	});
	return show;
}

void PoolController::recyclePurpleCrushShow(AnimationShowObject* anim) const
{
	anim->recycle();
	purpleCrushShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getRedCrushShow() const
{
	AnimationShowObject* show;
	if (redCrushShowPool->size() > 0)
	{
		show = redCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_red.csb");
	}
	show->reuse([=]()
	{
		this->recycleRedCrushShow(show);
	});
	return show;
}

void PoolController::recycleRedCrushShow(AnimationShowObject* anim) const
{
	anim->recycle();
	redCrushShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getYellowCrushShow() const
{
	AnimationShowObject* show;
	if (yellowCrushShowPool->size() > 0)
	{
		show = yellowCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_yellow.csb");
	}
	show->reuse([=]()
	{
		this->recycleYellowCrushShow(show);
	});
	return show;
}

void PoolController::recycleYellowCrushShow(AnimationShowObject* anim) const
{
	anim->recycle();
	yellowCrushShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getMatchCrushShow(TileColors color) const
{
	AnimationShowObject* ret = nullptr;
	switch (color)
	{
	case TileColors::blue:
		ret = getBlueCrushShow();
		break;
	case TileColors::green:
		ret = getGreenCrushShow();
		break;
	case TileColors::orange:
		ret = getOrangeCrushShow();
		break;
	case TileColors::purple:
		ret = getPurpleCrushShow();
		break;
	case TileColors::red:
		ret = getRedCrushShow();
		break;
	case TileColors::yellow:
		ret = getYellowCrushShow();
		break;
	default:
		ret = nullptr;
		break;
	}
	return ret;
}

void PoolController::recycleMatchCrushShow(AnimationShowObject* show, TileColors color) const
{
	switch (color)
	{
	case TileColors::blue:
		recycleBlueCrushShow(show);
		break;
	case TileColors::green:
		recycleGreenCrushShow(show);
		break;
	case TileColors::orange:
		recycleOrangeCrushShow(show);
		break;
	case TileColors::purple:
		recyclePurpleCrushShow(show);
		break;
	case TileColors::red:
		recycleRedCrushShow(show);
		break;
	case TileColors::yellow:
		recycleYellowCrushShow(show);
		break;
	default:
		break;
	}
}
