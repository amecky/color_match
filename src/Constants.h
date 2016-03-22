#pragma once
#include <utils\Color.h>
#include <math\math_types.h>

const float MOVE_TTL = 1.2f;
const int CELL_SIZE = 40;
const int HALF_CELL_SIZE = 20;
const int STARTX = 130;
const int STARTY = 144;
const int MAX_X = 20;
const int MAX_Y = 12;

const float MESSAGE_TTL = 0.8f;
const float MESSAGE_SCALE = 0.4f;

const ds::Rect BACKGROUND_RECT = ds::Rect(0,512,512,384);
const ds::Rect GRID = ds::Rect(0,40,80,80);

const ds::Rect TILE_RECT[] = {
	ds::Rect(680, 0,CELL_SIZE,CELL_SIZE),
	ds::Rect(680, 40,CELL_SIZE,CELL_SIZE),
	ds::Rect(680, 80,CELL_SIZE,CELL_SIZE),
	ds::Rect(680,120,CELL_SIZE,CELL_SIZE),
	ds::Rect(680,160,CELL_SIZE,CELL_SIZE),
};

const ds::Color COLOR_ARRAY[] = {
	ds::Color(103,35,35,255) ,
	ds::Color(41,35,43,255),
	ds::Color(117,61,44,255),
	ds::Color(83,98,94,255),
	ds::Color(114,110,101,255)
};





