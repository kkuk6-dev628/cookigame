
#pragma once
#include "enum.h"

BETTER_ENUM(GoalTypes, char, WaffleObject, PathObject, TopplingObject, PopsicleObject, HiderSegmentObject, HopplingObject, thoppling)

BETTER_ENUM(TileColors, char, blue, purple, yellow, green, orange, teal, pink, white, red, any, random)

BETTER_ENUM(ModifierTypes, char, NoShuffleModifier, CageModifier, HoneyModifier)

BETTER_ENUM(SetTypes, char, set, rnd)

BETTER_ENUM(HiderSegmentPosition, char, head, tail, body)

BETTER_ENUM(LayerId, char, Background = -1, UnderCover = 0, Cover = 1, PathCoveyor = 2, Waffle = 3, Match = 4, Toppling=5, Target=6, Portal=7, Spawner=8);

BETTER_ENUM(AdjacentDirs, char, 
	W = 0, // left
	E = 1, // right
	N = 2, // up
	S = 3, // down
	SE,
	SW,
	NE,
	NW,
	NoDir
	)

BETTER_ENUM(MovingTileTypes, char, 
	LayeredMatchObject = 0,
	BombBreakerObject,
	ColumnBreakerObject,
	RowBreakerObject,
	XBreakerObject,
	RainbowObject,
	SeekerObject,
	LiquidDrainerMatchObject,
	LiquidFillerMatchObject,
	PathMoverMatchObject,
	DonutObject
	)
