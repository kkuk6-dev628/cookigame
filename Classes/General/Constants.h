#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>

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

#define REWARDED_VIDEO_DEFAULT_TIME 900

#define PORTALLETHEIGHT 23

static const std::string EpisodeFilePrefix = "LevelsConfigEpisode";
static const char MaxCol = 9;
static const char MaxRow = 9;
static const char MaxTileCount = 9 * 9;
static const float TileMovingTime = 0.1f; 
extern char CellSize;
extern char MovingTileSize;
extern float CenterX;
extern float CenterY;

static const char AdjacentIndents[4][2] = { // {row, col}
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

#endif
