
#pragma once
#include "enum.h"

enum GameState {
	Idle, SwappingTile, Shuffling, Completed, Failed, Paused, Booster
};

enum MatchType
{
	NormalMatch, BonusMatch, RainbowMatch, SingleCellMatch
};

enum BoosterType
{
	BoosterHor,
	BoosterVer,
	BoosterSingle,
	BoosterSwap,
	//BoosterRainbow,
	BoosterCount,
	None
};

enum ScoreUnit
{
	obstacle = 20,
	match = 50,
	waffle = 30,
	pathFollower = 10000,
	popsicle = 5000,
	rowBreaker = 120,
	columnBreaker = 120,
	xBreaker = 200,
	seeker = 140,
	bombBreaker = 200,
	rainbow = 200,
	iceCover = 1000,
	thoppler = 5000,
	hiderSegment = 10000
};

enum ScoreType
{
	normal,
	additive
};

BETTER_ENUM(PopsicleType, char, normal, line, bomb, rainbow);

BETTER_ENUM(GoalTypes, char, WaffleObject, PathObject, TopplingObject, PopsicleObject, HiderSegmentObject, HopplingObject, thoppling, DigDownYumbleObject);

BETTER_ENUM(TileColors, char, blue, purple, yellow, green, orange, red, teal, pink, white, any, random);

BETTER_ENUM(ModifierTypes, char, NoShuffleModifier, CageModifier, HoneyModifier, DigDownModifier, None);

BETTER_ENUM(SetTypes, char, set, rnd);

BETTER_ENUM(HiderSegmentPosition, char, head, tail, body);

BETTER_ENUM(Direction, char, N, NE, E, SE, S, SW, W, NW, up, right, down, left, any);

BETTER_ENUM(LayerId, char, Background = -2, Border = -1, UnderCover = 0, Cover = 1, PathConveyor = 2, Waffle = 3, Match = 4, Toppling=5, Target=6, Portal=7, Spawner=8, ShowLayer=10);

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
);

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
	DonutObject,
	ChocolateChipObject,
	ChocolateCheesecakeObject,
	FixTile
);

BETTER_ENUM(SoundEffects, char,
	sound_gem_match_tier1,
	sound_gem_match_tier2,
	sound_gem_match_tier3,
	sound_gem_match_tier4,
	sound_gem_match_tier5,
	sound_gem_match_tier6,
	sound_gem_match_tier7,
	sound_gem_match_tier8,

	sound_gem_drag,
	sound_gem_drag_fail,
	sound_gem_fall,
	sound_gem_flying,
	sound_gem_landing,

	sound_create_x,
	sound_create_row_column,
	sound_create_rainbow,
	sound_create_honey,
	sound_create_bomb,

	sound_explode_bomb,
	sound_explode_cage,
	sound_explode_chocolate,
	sound_explode_chocolate_chip,
	sound_explode_donut,
	sound_explode_double_bomb,
	sound_explode_double_rainbow,
	sound_explode_honey,
	sound_explode_ice,
	sound_explode_rainbow,
	sound_explode_row_column,
	sound_explode_small_bomb,
	sound_explode_waffle,
	sound_explode_x,

	VO_toppler_move_1,
	VO_toppler_move_2,
	VO_toppler_move_3,
	VO_toppler_move_4,

	sound_gameboard_shuffle,
	sound_worm_chew
);

