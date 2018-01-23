#include "PoolController.h"
#include "Models/Tiles/LayeredMatchObject.h"
#include "Models/Tiles/LargeTile.h"
#include "Models/Tiles/FixTiles.h"
#include "Models/Tiles/SpawnerObject.h"
#include "Models/Tiles/ChocolateChipObject.h"

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

	blueSeekerShowPool = new NodePool<AnimationShowObject>;
	greenSeekerShowPool = new NodePool<AnimationShowObject>;
	orangeSeekerShowPool = new NodePool<AnimationShowObject>;
	purpleSeekerShowPool = new NodePool<AnimationShowObject>;
	redSeekerShowPool = new NodePool<AnimationShowObject>;
	yellowSeekerShowPool = new NodePool<AnimationShowObject>;

	chocolateCrushPool = new NodePool<AnimationShowObject>;

	chocolateChipPool = new NodePool<ChocolateChipObject>;
	chocolateChipCrushPool = new NodePool<AnimationShowObject>;

	donutPool = new NodePool<DonutObject>;
	donutCrushPool = new NodePool<AnimationShowObject>;

	lineCrushShowPool = new NodePool<AnimationShowObject>;
	bombAndLineCrushShowPool = new NodePool<AnimationShowObject>;

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
	REGISTER_CLASS(ChocolateObject);
	REGISTER_CLASS(PathObject);
	REGISTER_CLASS(PathFollowerObject);
	REGISTER_CLASS(PathGoalObject);
	REGISTER_CLASS(PortalInletObject);
	REGISTER_CLASS(PortalOutletObject);

	REGISTER_CLASS(InvisibleBrickObject);
	REGISTER_CLASS(EmptyObject);
	REGISTER_CLASS(SeekerPriorityObject);
}

CookieTile* PoolController::getCookieTile(std::string typeName)
{
	CookieTile* cookieTile;
	if (MovingTileTypes::_is_valid_nocase(typeName.c_str()))
	{
		auto tileType = MovingTileTypes::_from_string(typeName.c_str());
		switch(tileType)
		{
		case MovingTileTypes::ChocolateChipObject:
			if (chocolateChipPool->size() > 0)
			{
				cookieTile = chocolateChipPool->getNode();
			}
			else
			{
				cookieTile = new ChocolateChipObject();
			}
			break;
		case MovingTileTypes::DonutObject:
			if (donutPool->size() > 0)
			{
				cookieTile = donutPool->getNode();
			}
			else
			{
				cookieTile = new DonutObject();
			}
			break;
		default:
			if (tilesPool->size() > 0)
			{
				cookieTile = tilesPool->getTile();
			}
			else
			{
				cookieTile = new MovingTile();
			}
			break;
		}
	}
	else
	{
		cookieTile = static_cast<CookieTile*>(TileClassFactory.construct(typeName));
	}
	if (cookieTile != nullptr) 
	{
		cookieTile->setType(typeName);
	}
	return cookieTile;
}

void PoolController::recycleCookieTile(CookieTile* cookieTile) const
{
	if (MovingTileTypes::_is_valid_nocase(cookieTile->getType().c_str()))
	{
		auto tileType = cookieTile->getMovingTileType();
		switch(tileType)
		{
		case MovingTileTypes::ChocolateChipObject:
			chocolateChipPool->recycleNode(static_cast<ChocolateChipObject*>(cookieTile));
			break;
		case MovingTileTypes::DonutObject:
			donutPool->recycleNode(static_cast<DonutObject*>(cookieTile));
			break;
		default:
			tilesPool->putTile(static_cast<MovingTile*>(cookieTile));
			break;
		}
	}
	else
	{
		cookieTile->removeFromParent();
		CC_SAFE_RELEASE(cookieTile);
	}

}

//ChocolateChipObject* PoolController::getChocolateChip() const
//{
//}
//
//void PoolController::recycleChocolateChip(ChocolateChipObject* show) const
//{
//
//}

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
		show = new ParticleShowObject;
		show->initWithCSB("res/particle/tail_particle_blue_G.csb");
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
		show = new ParticleShowObject;
		show->initWithCSB("res/particle/tail_particle_green_G.csb");
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
		show = new ParticleShowObject;
		show->initWithCSB("res/particle/tail_particle_orange_G.csb");
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
		show = new ParticleShowObject;
		show->initWithCSB("res/particle/tail_particle_purple_G.csb");
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
		show = new ParticleShowObject;
		show->initWithCSB("res/particle/tail_particle_red_G.csb");
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
		show = new ParticleShowObject;
		show->initWithCSB("res/particle/tail_particle_yellow_G.csb");
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

AnimationShowObject* PoolController::getBlueSeekerShow() const
{
	AnimationShowObject* show;
	if (blueSeekerShowPool->size() > 0)
	{
		show = blueSeekerShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/animation/bee_blue.csb");
	}
	show->reuse(nullptr);
	return show;
}

void PoolController::recycleBlueSeekerShow(AnimationShowObject* anim) const
{
	anim->recycle();
	blueSeekerShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getGreenSeekerShow() const
{
	AnimationShowObject* show;
	if (greenSeekerShowPool->size() > 0)
	{
		show = greenSeekerShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/animation/bee_green.csb");
	}
	show->reuse(nullptr);
	return show;
}

void PoolController::recycleGreenSeekerShow(AnimationShowObject* anim) const
{
	anim->recycle();
	greenSeekerShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getOrangeSeekerShow() const
{
	AnimationShowObject* show;
	if (orangeSeekerShowPool->size() > 0)
	{
		show = orangeSeekerShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/animation/bee_orange.csb");
	}
	show->reuse(nullptr);
	return show;
}

void PoolController::recycleOrangeSeekerShow(AnimationShowObject* anim) const
{
	anim->recycle();
	orangeSeekerShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getPurpleSeekerShow() const
{
	AnimationShowObject* show;
	if (purpleSeekerShowPool->size() > 0)
	{
		show = purpleSeekerShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/animation/bee_purple.csb");
	}
	show->reuse(nullptr);
	return show;
}

void PoolController::recyclePurpleSeekerShow(AnimationShowObject* anim) const
{
	anim->recycle();
	purpleSeekerShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getRedSeekerShow() const
{
	AnimationShowObject* show;
	if (redSeekerShowPool->size() > 0)
	{
		show = redSeekerShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/animation/bee_red.csb");
	}
	show->reuse(nullptr);
	return show;
}

void PoolController::recycleRedSeekerShow(AnimationShowObject* anim) const
{
	anim->recycle();
	redSeekerShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getYellowSeekerShow() const
{
	AnimationShowObject* show;
	if (yellowSeekerShowPool->size() > 0)
	{
		show = yellowSeekerShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/animation/bee_yellow.csb");
	}
	show->reuse(nullptr);
	return show;
}

void PoolController::recycleYellowSeekerShow(AnimationShowObject* anim) const
{
	anim->recycle();
	yellowSeekerShowPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getSeekerShow(TileColors color) const
{
	AnimationShowObject* ret = nullptr;
	switch (color)
	{
	case TileColors::blue:
		ret = getBlueSeekerShow();
		break;
	case TileColors::green:
		ret = getGreenSeekerShow();
		break;
	case TileColors::orange:
		ret = getOrangeSeekerShow();
		break;
	case TileColors::purple:
		ret = getPurpleSeekerShow();
		break;
	case TileColors::red:
		ret = getRedSeekerShow();
		break;
	case TileColors::yellow:
		ret = getYellowSeekerShow();
		break;
	default:
		ret = nullptr;
		break;
	}
	ret->tileColor = color;
	return ret;
}

void PoolController::recycleSeekerShow(AnimationShowObject* show) const
{
	switch (show->tileColor)
	{
	case TileColors::blue:
		recycleBlueSeekerShow(show);
		break;
	case TileColors::green:
		recycleGreenSeekerShow(show);
		break;
	case TileColors::orange:
		recycleOrangeSeekerShow(show);
		break;
	case TileColors::purple:
		recyclePurpleSeekerShow(show);
		break;
	case TileColors::red:
		recycleRedSeekerShow(show);
		break;
	case TileColors::yellow:
		recycleYellowSeekerShow(show);
		break;
	default:
		break;
	}
}

AnimationShowObject* PoolController::getChocolateCrushShow() const
{
	AnimationShowObject* show;
	if (chocolateCrushPool->size() > 0)
	{
		show = chocolateCrushPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/Chocolate_0.csb");
	}
	show->reuse([=]()
	{
		this->recycleChocolateCrushShow(show);
	});
	return show;
}

void PoolController::recycleChocolateCrushShow(AnimationShowObject* anim) const
{
	anim->recycle();
	chocolateCrushPool->recycleNode(anim);
}

AnimationShowObject* PoolController::getChocolateChipCrushShow() const
{
	AnimationShowObject* show;
	if (chocolateChipCrushPool->size() > 0)
	{
		show = chocolateChipCrushPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/ChocolateChip.csb");
	}
	show->reuse([=]()
	{
		this->recycleChocolateChipCrushShow(show);
	});
	return show;
}

void PoolController::recycleChocolateChipCrushShow(AnimationShowObject* show) const
{
	show->recycle();
	chocolateChipCrushPool->recycleNode(show);
}

AnimationShowObject* PoolController::getDonutCrushShow() const
{
	AnimationShowObject* show;
	if (donutCrushPool->size() > 0)
	{
		show = donutCrushPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/Donut.csb");
	}
	show->reuse([=]()
	{
		this->recycleDonutCrushShow(show);
	});
	return show;
}

void PoolController::recycleDonutCrushShow(AnimationShowObject* show) const
{
	show->recycle();
	donutCrushPool->recycleNode(show);
}

AnimationShowObject* PoolController::getLineCrushShow() const
{
	AnimationShowObject* show;
	if (lineCrushShowPool->size() > 0)
	{
		show = lineCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_light_2.csb");
	}
	show->reuse([=]()
	{
		this->recycleLineCrushShow(show);
	});
	return show;
}

void PoolController::recycleLineCrushShow(AnimationShowObject* show) const
{
	show->recycle();
	lineCrushShowPool->recycleNode(show);
}

AnimationShowObject* PoolController::getBombAndLineCrushShow() const
{
	AnimationShowObject* show;
	if (bombAndLineCrushShowPool->size() > 0)
	{
		show = bombAndLineCrushShowPool->getNode();
	}
	else
	{
		show = new AnimationShowObject;
		show->initWithCSB("res/particle/particle_light_3.csb");
	}
	show->reuse([=]()
	{
		this->recycleBombAndLineCrushShow(show);
	});
	return show;
}

void PoolController::recycleBombAndLineCrushShow(AnimationShowObject* show) const
{
	show->recycle();
	bombAndLineCrushShowPool->recycleNode(show);
}
