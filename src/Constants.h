#pragma once
#include <utils\Color.h>
#include <math\math_types.h>

const int CELL_SIZE = 40;
const int HALF_CELL_SIZE = 20;
const int STARTX = 130;
const int STARTY = 144;
const int MAX_X = 20;
const int MAX_Y = 12;


enum BoardEvents {
	BE_NONE,
	BE_INVALID_SELECTION,
	BE_SCORE,
	BE_EOL
};

enum GameEvents {
	GE_BASIC,
	GE_SHOW_TEST_GUI,
	GE_HIDE_TEST_GUI,
	GE_REBUILD_BOARD,
	GE_END_GAME,
	GE_TOGGLE_SHOW_BOARD,
	GE_TOGGLE_SHOW_STATES,
	GE_FADE_TO_GRAY,
	GE_TOGGLE_SETTINGS
};


