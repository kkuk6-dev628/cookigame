#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>

static const std::string EpisodeFilePrefix = "LevelsConfigEpisode";
static const char MaxCol = 9;
static const char MaxRow = 9;
static const char MaxTileCount = 9 * 9;
static const float TileMovingTime = 0.1f; 
extern char CellSize;
extern float CenterX;
extern float CenterY;

static const char AdjacentIndents[4][2] = { // {row, col}
		{0, -1},	// left
		{0, 1},		// right
		{-1, 0},	// down
		{1, 0},		// up
};
#endif
