#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <map>
#include <vector>

// design
#define SCREEN_X	720.f
//768.f
#define SCREEN_Y	1280.f
//1356.f

// level
#define MAX_TARGET	4
#define MAX_STAR	3
#define MAX_COLOR	6
// board
#define MAX_ROW		9
#define MAX_COL		9

#define HINT_TIME 5.0F // second

#define REWARDED_VIDEO_DEFAULT_TIME 900

#define PORTALLETHEIGHT 23

#define HINT_ACTION 3001

// Keys for special tiles

#define BREAKERS "breakers"
#define WAFFLEPATH "wafflePath"
#define LIQUIDS "liquids"
#define THOPPLERS "thopplers"

// tile objects

#define TOPPLINGOBJECT "TopplingObject"
#define HOPPLINGOBJECT "HopplingObject"
#define CRACKEROBJECT "CrackerObject"
#define LAVACAKEOBJECT "LavaCakeObject"
#define LAVACAKETARGETOBJECT "LavaCakeTargetObject"
#define SEEKERPRIORITYOBJECT "SeekerPriorityObject"
#define EMPTYOBJECT "EmptyObject"
#define INVISIBLEBRICKOBJECT "InvisibleBrickObject"
#define PORTALOUTLETOBJECT "PortalOutletObject"
#define PORTALINLETOBJECT "PortalInletObject"
#define POPSICLEOBJECT "PopsicleObject"
#define POINTEROBJECT "PointerObject"
#define OBJECTSPINNEROBJECT "ObjectSpinnerObject"
#define WAFFLEOBJECT "WaffleObject"
#define PIECESWAPPEROBJECT "PieceSwapperObject"
#define PRETZELOBJECT "PretzelObject"
#define SPAWNEROBJECT "SpawnerObject"
#define DISPLAYCASEOBJECT "DisplayCaseObject"

//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Tiles ZOrders

#define PRETZELZORDER 10000

//////////////////////////////////////////////////

static const std::string EpisodeFilePrefix = "LevelsConfigEpisode";
static const char MaxCol = 9;
static const char MaxRow = 9;
static const char MaxTileCount = 9 * 9;
static const float TileMovingTime = 0.1f; 
extern char CellSize;
extern char MovingTileSize;
extern float CenterX;
extern float CenterY;

static char AdjacentIndents[4][2] = { // {row, col}
		{0, -1},	// left
		{0, 1},		// right
		{-1, 0},	// down
		{1, 0},		// up
};

#define AVAILABLE_MOVES_COUNT 16
static const char AvailableMoves[AVAILABLE_MOVES_COUNT][4][2] = {
	{
		{0, 1}, {1, 2}, {0, 2}, {1, 2}
	},
	{
		{ 0, 1 },{ -1, 2 },{ 0, 2 },{ -1, 2 }
	},
	{
		{ 0, -1 },{ 1, -2 },{ 0, -2 },{ 1, -2 }
	},
	{
		{ 0, -1 },{ -1, -2 },{ 0, -2 },{ -1, -2 }
	},
	{
		{ 1, 0 },{ 2, 1 },{ 2, 0 },{ 2, 1 }
	},
	{
		{ 1, 0 },{ 2, -1 },{ 2, 0 },{ 2, -1 }
	},
	{
		{ -1, 0 },{ -2, 1 },{ -2, 0 },{ -2, 1 }
	},
	{
		{ -1, 0 },{ -2, -1 },{ -2, 0 },{ -2, -1 }
	},
	{
		{ 1, 1 },{ 1, -1 },{ 1, 0 },{ 0, 0 }
	},
	{
		{ -1, 1 },{ -1, -1 },{ -1, 0 },{ 0, 0 }
	},
	{
		{ 1, 1 },{ -1, 1 },{ 0, 1 },{ 0, 0 }
	},
	{
		{ 1, -1 },{ -1, -1 },{ 0, -1 },{ 0, 0 }
	},
	{
		{ 0, 1 },{ 0, -2 },{ 0, -1 },{ 0, -2 }
	},
	{
		{ 0, -1 },{ 0, 2 },{ 0, 1 },{ 0, 2 }
	},
	{
		{ 1, 0 },{ -2, 0 },{ -1, 0 },{ -2, 0 }
	},
	{
		{ -1, 0 },{ 2, 0 },{ 1, 0 },{ 2, 0 }
	},
};

static const std::map<std::string, std::vector<char>> MoveDirectionsMap = { // { col, row }
	{ std::string("down"),{ -1, 0 } },
	{ std::string("down-left"),{ -1, 0 } },
	{ std::string("down-left (CW)"),{ 0, -1 } },
	{ std::string("down-right"),{ 0, 1 } },
	{ std::string("down-right (CW)"),{ -1, 0 } },
	{ std::string("left"),{ 0, -1 } },
	{ std::string("right"),{ 0, 1 } },
	{ std::string("up"),{ 1, 0 } },
	{ std::string("up-left"),{ 0, -1 } },
	{ std::string("up-left (CW)"),{ 1, 0 } },
	{ std::string("up-right"),{ 1, 0 } },
	{ std::string("up-right (CW)"),{ 0, 1 } },
};
#endif
